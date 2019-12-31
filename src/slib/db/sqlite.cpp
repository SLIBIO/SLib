/*
 *   Copyright (c) 2008-2019 SLIBIO <https://github.com/SLIBIO>
 *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *   THE SOFTWARE.
 */

#include "sqlite/sqlite3.h"

#include "slib/db/sqlite.h"

#include "slib/db/sql.h"
#include "slib/core/file.h"
#include "slib/core/log.h"
#include "slib/core/scoped.h"
#include "slib/core/safe_static.h"
#include "slib/crypto/sha2.h"
#include "slib/crypto/chacha.h"

#define TAG "SQLiteDatabase"

#define ENCRYPTION_PREFIX_SIZE 80

namespace slib
{	

	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(SQLiteParam)

	SQLiteParam::SQLiteParam()
	{
		flagCreate = sl_true;
		flagReadonly = sl_false;
	}

	SLIB_DEFINE_OBJECT(SQLiteDatabase, Database)

	SQLiteDatabase::SQLiteDatabase()
	{
		m_dialect = DatabaseDialect::SQLite;
	}

	SQLiteDatabase::~SQLiteDatabase()
	{
	}

	namespace priv
	{
		namespace sqlite
		{
		
			class DatabaseImpl;
		
			class CursorImpl : public DatabaseCursor
			{
			public:
				Ref<DatabaseStatement> m_statementObj;
				sqlite3_stmt* m_statement;

				CList<String> m_listColumnNames;
				sl_uint32 m_nColumnNames;
				String* m_columnNames;
				CHashMap<String, sl_int32> m_mapColumnIndexes;

			public:
				CursorImpl(Database* db, DatabaseStatement* statementObj, sqlite3_stmt* statement)
				{
					m_db = db;
					m_statementObj = statementObj;
					m_statement = statement;

					sl_int32 cols = sqlite3_column_count(statement);
					for (sl_int32 i = 0; i < cols; i++) {
						const char* buf = sqlite3_column_name(statement, (int)i);
						String name = String::create(buf);
						m_listColumnNames.add_NoLock(name);
						m_mapColumnIndexes.put_NoLock(name, i);
					}
					m_nColumnNames = (sl_uint32)(m_listColumnNames.getCount());
					m_columnNames = m_listColumnNames.getData();

					db->lock();
				}

				~CursorImpl()
				{
					sqlite3_reset(m_statement);
					sqlite3_clear_bindings(m_statement);
					m_db->unlock();
				}

			public:
				sl_uint32 getColumnsCount() override
				{
					return m_nColumnNames;
				}

				String getColumnName(sl_uint32 index) override
				{
					if (index < m_nColumnNames) {
						return m_columnNames[index];
					}
					return sl_null;
				}

				sl_int32 getColumnIndex(const StringParam& name) override
				{
					return m_mapColumnIndexes.getValue_NoLock(name.toString(), -1);
				}

				HashMap<String, Variant> getRow() override
				{
					HashMap<String, Variant> ret;
					if (m_nColumnNames > 0) {
						for (sl_uint32 index = 0; index < m_nColumnNames; index++) {
							ret.put_NoLock(m_columnNames[index], _getValue(index));
						}
					}
					return ret;
				}

				String _getString(sl_uint32 index)
				{
					int n = sqlite3_column_bytes(m_statement, index);
					const void* buf = sqlite3_column_text(m_statement, index);
					if (buf && n > 0) {
						return String::fromUtf8(buf, n);
					}
					return String::getEmpty();
				}

				Memory _getBlob(sl_uint32 index)
				{
					int n = sqlite3_column_bytes(m_statement, index);
					const void* buf = sqlite3_column_blob(m_statement, index);
					if (buf && n > 0) {
						return Memory::create(buf, n);
					}
					return sl_null;
				}

				Variant _getValue(sl_uint32 index)
				{
					int type = sqlite3_column_type(m_statement, index);
					switch (type) {
					case SQLITE_INTEGER:
						{
							sl_int64 v64 = sqlite3_column_int64(m_statement, index);
							sl_int32 v32 = (sl_int32)v64;
							if (v64 == v32) {
								return v32;
							} else {
								return v64;
							}
						}
						break;
					case SQLITE_FLOAT:
						return sqlite3_column_double(m_statement, index);
					case SQLITE_TEXT:
						return _getString(index);
					case SQLITE_BLOB:
						return _getBlob(index);
					}
					return sl_null;
				}

				Variant getValue(sl_uint32 index) override
				{
					if (index < m_nColumnNames) {
						return _getValue(index);
					}
					return sl_null;
				}

				String getString(sl_uint32 index) override
				{
					if (index < m_nColumnNames) {
						int type = sqlite3_column_type(m_statement, index);
						switch (type) {
						case SQLITE_INTEGER:
							return String::fromInt64(sqlite3_column_int64(m_statement, index));
						case SQLITE_FLOAT:
							return String::fromDouble(sqlite3_column_double(m_statement, index));
						case SQLITE_TEXT:
							return _getString(index);
						}
					}
					return sl_null;
				}

				sl_int64 getInt64(sl_uint32 index, sl_int64 defaultValue) override
				{
					if (index < m_nColumnNames) {
						int type = sqlite3_column_type(m_statement, index);
						switch (type) {
						case SQLITE_INTEGER:
							return sqlite3_column_int64(m_statement, index);
						case SQLITE_FLOAT:
							return (sl_int64)(sqlite3_column_double(m_statement, index));
						case SQLITE_TEXT:
							return _getString(index).parseInt64(10, defaultValue);
						}
					}
					return defaultValue;
				}

				sl_uint64 getUint64(sl_uint32 index, sl_uint64 defaultValue) override
				{
					if (index < m_nColumnNames) {
						int type = sqlite3_column_type(m_statement, index);
						switch (type) {
						case SQLITE_INTEGER:
							return sqlite3_column_int64(m_statement, index);
						case SQLITE_FLOAT:
							return (sl_uint64)(sqlite3_column_double(m_statement, index));
						case SQLITE_TEXT:
							return _getString(index).parseUint64(10, defaultValue);
						}
					}
					return defaultValue;
				}

				sl_int32 getInt32(sl_uint32 index, sl_int32 defaultValue) override
				{
					if (index < m_nColumnNames) {
						int type = sqlite3_column_type(m_statement, index);
						switch (type) {
						case SQLITE_INTEGER:
							return sqlite3_column_int(m_statement, index);
						case SQLITE_FLOAT:
							return (sl_int32)(sqlite3_column_double(m_statement, index));
						case SQLITE_TEXT:
							return _getString(index).parseInt32(10, defaultValue);
						}
					}
					return defaultValue;
				}

				sl_uint32 getUint32(sl_uint32 index, sl_uint32 defaultValue) override
				{
					if (index < m_nColumnNames) {
						int type = sqlite3_column_type(m_statement, index);
						switch (type) {
						case SQLITE_INTEGER:
							return (sl_uint32)(sqlite3_column_int64(m_statement, index));
						case SQLITE_FLOAT:
							return (sl_uint32)(sqlite3_column_double(m_statement, index));
						case SQLITE_TEXT:
							return _getString(index).parseUint32(10, defaultValue);
						}
					}
					return defaultValue;
				}

				float getFloat(sl_uint32 index, float defaultValue) override
				{
					if (index < m_nColumnNames) {
						int type = sqlite3_column_type(m_statement, index);
						switch (type) {
						case SQLITE_INTEGER:
							return (float)(sqlite3_column_int64(m_statement, index));
						case SQLITE_FLOAT:
							return (float)(sqlite3_column_double(m_statement, index));
						case SQLITE_TEXT:
							return _getString(index).parseFloat(defaultValue);
						}
					}
					return defaultValue;
				}

				double getDouble(sl_uint32 index, double defaultValue) override
				{
					if (index < m_nColumnNames) {
						int type = sqlite3_column_type(m_statement, index);
						switch (type) {
						case SQLITE_INTEGER:
							return (double)(sqlite3_column_int64(m_statement, index));
						case SQLITE_FLOAT:
							return (sqlite3_column_double(m_statement, index));
						case SQLITE_TEXT:
							return _getString(index).parseDouble(defaultValue);
						}
					}
					return defaultValue;
				}

				Memory getBlob(sl_uint32 index) override
				{
					if (index < m_nColumnNames) {
						int type = sqlite3_column_type(m_statement, index);
						switch (type) {
						case SQLITE_TEXT:
						case SQLITE_BLOB:
							return _getBlob(index);
						}
					}
					return sl_null;
				}

				sl_bool moveNext() override
				{
					sl_int32 nRet = sqlite3_step(m_statement);
					if (nRet == SQLITE_ROW) {
						return sl_true;
					}
					return sl_false;
				}

			};

			class StatementImpl : public DatabaseStatement
			{
			public:
				sqlite3* m_sqlite;
				sqlite3_stmt* m_statement;
				Array<Variant> m_boundParams;

			public:
				StatementImpl(Database* db, sqlite3* sqlite, sqlite3_stmt* statement)
				{
					m_db = db;
					m_sqlite = sqlite;
					m_statement = statement;
				}

				~StatementImpl()
				{
					sqlite3_finalize(m_statement);
				}
				
			public:
				sl_bool isLoggingErrors()
				{
					if (m_db.isNotNull()) {
						return m_db->isLoggingErrors();
					}
					return sl_false;
				}
				
				sl_bool _execute(const Variant* _params, sl_uint32 nParams)
				{
					sqlite3_reset(m_statement);
					sqlite3_clear_bindings(m_statement);
					m_boundParams.setNull();
					
					if (nParams == 0) {
						return sl_true;
					}

					Array<Variant> params = Array<Variant>::create(_params, nParams);
					if (params.isNull()) {
						return sl_false;
					}
					sl_uint32 n = (sl_uint32)(sqlite3_bind_parameter_count(m_statement));
					if (n == nParams) {
						if (n > 0) {
							for (sl_uint32 i = 0; i < n; i++) {
								int iRet = SQLITE_ABORT;
								Variant& var = (params.getData())[i];
								switch (var.getType()) {
								case VariantType::Null:
									iRet = sqlite3_bind_null(m_statement, i+1);
									break;
								case VariantType::Boolean:
								case VariantType::Int32:
									iRet = sqlite3_bind_int(m_statement, i+1, var.getInt32());
									break;
								case VariantType::Uint32:
								case VariantType::Int64:
								case VariantType::Uint64:
									iRet = sqlite3_bind_int64(m_statement, i+1, var.getInt64());
									break;
								case VariantType::Float:
								case VariantType::Double:
									iRet = sqlite3_bind_double(m_statement, i+1, var.getDouble());
									break;
								default:
									if (var.isMemory()) {
										Memory mem = var.getMemory();
										sl_size size = mem.getSize();
										if (size > 0x7fffffff) {
											iRet = sqlite3_bind_blob64(m_statement, i+1, mem.getData(), size, SQLITE_STATIC);
										} else {
											iRet = sqlite3_bind_blob(m_statement, i+1, mem.getData(), (sl_uint32)size, SQLITE_STATIC);
										}
									} else {
										String str = var.getString();
										var = str;
										iRet = sqlite3_bind_text(m_statement, i+1, str.getData(), (sl_uint32)(str.getLength()), SQLITE_STATIC);
									}
								}
								if (iRet != SQLITE_OK) {
									return sl_false;
								}
							}
						}
						m_boundParams = params;
						return sl_true;
					} else {
						if (isLoggingErrors()) {
							LogError(TAG, "Bind error: requires %d params but %d params provided", n, nParams);
						}
					}
					return sl_false;
				}

				sl_int64 executeBy(const Variant* params, sl_uint32 nParams) override
				{
					ObjectLocker lock(m_db.get());
					if (_execute(params, nParams)) {
						if (sqlite3_step(m_statement) == SQLITE_DONE) {
							sqlite3_reset(m_statement);
							sqlite3_clear_bindings(m_statement);
							return sqlite3_changes(m_sqlite);
						}
					}
					return -1;
				}

				Ref<DatabaseCursor> queryBy(const Variant* params, sl_uint32 nParams) override
				{
					ObjectLocker lock(m_db.get());
					Ref<DatabaseCursor> ret;
					if (_execute(params, nParams)) {
						ret = new CursorImpl(m_db.get(), this, m_statement);
						if (ret.isNotNull()) {
							return ret;
						}
						sqlite3_reset(m_statement);
						sqlite3_clear_bindings(m_statement);
					}
					return ret;
				}
			};

		
			class EncryptionVfs : public sqlite3_vfs
			{
			public:
				DatabaseImpl* db;
			};
		
			class EncryptionIo : public sqlite3_io_methods
			{
			public:
				DatabaseImpl* db;
				const sqlite3_io_methods* ioOriginal;
				ChaCha20_Core encrypt;
				sl_uint32 encryptIV[4];
			};
		
			struct EncryptionCustomFile
			{
				EncryptionIo io;
			};
		
			class DatabaseImpl : public SQLiteDatabase
			{
			public:
				sqlite3* m_db;
				
				EncryptionVfs m_vfs;
				sqlite3_vfs* m_vfsOriginal;
				int m_vfsFileCustomOffset;
				char m_encryptionKey[32];

			public:
				DatabaseImpl()
				{
					m_db = sl_null;
				}

				~DatabaseImpl()
				{
					if (m_db) {
						sqlite3_close(m_db);
					}
				}

				static Ref<DatabaseImpl> open(const SQLiteParam& param)
				{
					Ref<DatabaseImpl> ret = new DatabaseImpl;
					if (ret.isNotNull()) {
						if (ret->initialize(param)) {
							return ret;
						}
					}
					return sl_null;
				}
				
				sl_bool initialize(const SQLiteParam& param)
				{
					sqlite3* db = sl_null;
					int flags;
					if (param.flagCreate) {
						flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
					} else {
						flags = param.flagReadonly ? SQLITE_OPEN_READONLY : SQLITE_OPEN_READWRITE;
					}
					
					int iResult;
					if (param.encryptionKey.isNotEmpty()) {
						SHA256::hash(param.encryptionKey, m_encryptionKey);
						sqlite3_vfs* vfsDefault = sqlite3_vfs_find(0);
						Base::copyMemory(&m_vfs, vfsDefault, sizeof(sqlite3_vfs));
						m_vfs.db = this;
						m_vfs.zName = "slib_encryption";
						m_vfs.xOpen = &xOpenEncryption;
						m_vfsFileCustomOffset = ((m_vfs.szOsFile - 1) | 15) + 1;
						m_vfs.szOsFile = m_vfsFileCustomOffset + sizeof(EncryptionCustomFile);
						m_vfsOriginal = vfsDefault;
						SLIB_SAFE_STATIC(Mutex, mutex)
						MutexLocker lock(&mutex);
						sqlite3_vfs_register(&m_vfs, 0);
						iResult = sqlite3_open_v2(param.path.getData(), &db, flags, m_vfs.zName);
						sqlite3_vfs_unregister(&m_vfs);
					} else {
						iResult = sqlite3_open_v2(param.path.getData(), &db, flags, sl_null);
					}
					if (SQLITE_OK == iResult) {
						m_db = db;
						return sl_true;
					}
					return sl_false;
				}
				
				static int xOpenEncryption(sqlite3_vfs* vfs, const char *zName, sqlite3_file* file, int flags, int *pOutFlags)
				{
					return ((EncryptionVfs*)vfs)->db->onOpenEncryption(vfs, zName, file, flags, pOutFlags);
				}
				
				int onOpenEncryption(sqlite3_vfs* vfs, const char *zName, sqlite3_file* file, int flags, int *pOutFlags)
				{
					int iRet = (m_vfsOriginal->xOpen)(vfs, zName, file, flags, pOutFlags);
					if (iRet == SQLITE_OK) {
						EncryptionCustomFile* custom = (EncryptionCustomFile*)(((char*)(void*)file) + m_vfsFileCustomOffset);
						Base::copyMemory(&(custom->io), file->pMethods, sizeof(sqlite3_io_methods));
						custom->io.db = this;
						custom->io.ioOriginal = file->pMethods;
						custom->io.iVersion = 0;
						custom->io.xRead = &xReadEncryption;
						custom->io.xWrite = &xWriteEncryption;
						custom->io.xTruncate = &xTruncateEncryption;
						custom->io.xFileSize = &xFileSizeEncryption;
						sqlite3_int64 size = 0;
						(file->pMethods->xFileSize)(file, &size);
						if (!size) {
							(vfs->xRandomness)(vfs, 4, (char*)(custom->io.encryptIV));
							(vfs->xRandomness)(vfs, 4, (char*)(custom->io.encryptIV + 1));
							(vfs->xRandomness)(vfs, 4, (char*)(custom->io.encryptIV + 2));
							(vfs->xRandomness)(vfs, 4, (char*)(custom->io.encryptIV + 3));
							char header[ENCRYPTION_PREFIX_SIZE];
							MIO::writeUint32LE(header, custom->io.encryptIV[0]);
							MIO::writeUint32LE(header + 4, custom->io.encryptIV[1]);
							MIO::writeUint32LE(header + 8, custom->io.encryptIV[2]);
							MIO::writeUint32LE(header + 12, custom->io.encryptIV[3]);
							(vfs->xRandomness)(vfs, 32, header + 16);
							char check[48];
							Base::copyMemory(check, header, 16);
							Base::copyMemory(check + 16, m_encryptionKey, 32);
							char h[32];
							SHA256::hash(check, 48, h);
							Base::copyMemory(header + 48, h, 32);
							iRet = (file->pMethods->xWrite)(file, header, ENCRYPTION_PREFIX_SIZE, 0);
							if (iRet != SQLITE_OK) {
								file->pMethods = sl_null;
								return iRet;
							}
							char key[32];
							for (int i = 0; i < 32; i++) {
								key[i] = header[16 + i] ^ m_encryptionKey[i];
							}
							custom->io.encrypt.setKey(key, 32);
						} else {
							char header[ENCRYPTION_PREFIX_SIZE];
							iRet = (file->pMethods->xRead)(file, header, ENCRYPTION_PREFIX_SIZE, 0);
							if (iRet != SQLITE_OK) {
								file->pMethods = sl_null;
								return iRet;
							}
							char check[48];
							Base::copyMemory(check, header, 16);
							Base::copyMemory(check + 16, m_encryptionKey, 32);
							char h[32];
							SHA256::hash(check, 48, h);
							if (!(Base::equalsMemory(h, header + 48, 32))) {
								file->pMethods = sl_null;
								return SQLITE_AUTH;
							}
							custom->io.encryptIV[0] = MIO::readUint32LE(header);
							custom->io.encryptIV[1] = MIO::readUint32LE(header + 4);
							custom->io.encryptIV[2] = MIO::readUint32LE(header + 8);
							custom->io.encryptIV[3] = MIO::readUint32LE(header + 12);
							char key[32];
							for (int i = 0; i < 32; i++) {
								key[i] = header[16 + i] ^ m_encryptionKey[i];
							}
							custom->io.encrypt.setKey(key, 32);
						}
						file->pMethods = &(custom->io);
					}
					return iRet;
				}
				
				static int xReadEncryption(sqlite3_file* file, void* buf, int iAmt, sqlite3_int64 iOfst)
				{
					EncryptionIo* io = (EncryptionIo*)(file->pMethods);
					int iRet = (io->ioOriginal->xRead)(file, buf, iAmt, iOfst + ENCRYPTION_PREFIX_SIZE);
					if (iRet == SQLITE_OK && iAmt > 0) {
						io->db->encrypt(io, iOfst, buf, iAmt);
					}
					return iRet;
				}
				
				static int xWriteEncryption(sqlite3_file* file, const void* buf, int iAmt, sqlite3_int64 iOfst)
				{
					EncryptionIo* io = (EncryptionIo*)(file->pMethods);
					if (iAmt > 0) {
						int n = iAmt >> 10;
						if (iAmt & 1023) {
							n++;
						}
						char* b = (char*)buf;
						char t[1024];
						sqlite3_int64 o = iOfst;
						for (int i = 0; i < n; i++) {
							int size = iAmt;
							if (size > 1024) {
								size = 1024;
							}
							Base::copyMemory(t, b, (sl_size)size);
							io->db->encrypt(io, o, t, size);
							int iRet = (io->ioOriginal->xWrite)(file, t, size, o + ENCRYPTION_PREFIX_SIZE);
							if (iRet != SQLITE_OK) {
								return iRet;
							}
							o += 1024;
							b += 1024;
							iAmt -= 1024;
						}
					}
					return SQLITE_OK;
				}
				
				static int xTruncateEncryption(sqlite3_file* file, sqlite3_int64 size)
				{
					EncryptionIo* io = (EncryptionIo*)(file->pMethods);
					return (io->ioOriginal->xTruncate)(file, size + ENCRYPTION_PREFIX_SIZE);
				}
				
				static int xFileSizeEncryption(sqlite3_file* file, sqlite3_int64 *pSize)
				{
					EncryptionIo* io = (EncryptionIo*)(file->pMethods);
					int iResult = (io->ioOriginal->xFileSize)(file, pSize);
					if (iResult == SQLITE_OK) {
						if (*pSize > ENCRYPTION_PREFIX_SIZE) {
							*pSize -= ENCRYPTION_PREFIX_SIZE;
						} else {
							*pSize = 0;
						}
					}
					return iResult;
				}
				
				void encrypt(EncryptionIo* io, sqlite3_int64 iOfst, void* buf, int size)
				{
					if (iOfst < 0) {
						return;
					}
					if (size < 1) {
						return;
					}
					sl_uint64 offset = (sl_uint64)iOfst;
					sl_uint64 block = offset >> 6;
					sl_uint32 pos = (sl_uint32)(offset & 63);
					sl_uint64 offsetEnd = (sl_uint64)(iOfst + size);
					sl_uint64 blockEnd = offsetEnd >> 6;
					sl_uint32 posEnd = (sl_uint32)(offsetEnd & 63);
					char* b = (char*)buf;
					char h[64];
					for (; block <= blockEnd; block++) {
						io->encrypt.generateBlock(io->encryptIV[0], io->encryptIV[1], io->encryptIV[2] + ((sl_uint32)(block >> 32)), io->encryptIV[3] + ((sl_uint32)block), h);
						sl_uint32 e;
						if (block == blockEnd) {
							e = posEnd;
						} else {
							e = 64;
						}
						sl_uint32 n = e - pos;
						for (sl_uint32 i = 0; i < n; i++) {
							b[i] ^= h[pos + i];
						}
						b += n;
						pos = 0;
					}
				}
				
				sl_int64 _execute(const StringParam& _sql) override
				{
					StringCstr sql(_sql);
					ObjectLocker lock(this);
					if (SQLITE_OK == sqlite3_exec(m_db, sql.getData(), 0, 0, sl_null)) {
						return sqlite3_changes(m_db);
					}
					return -1;
				}

				Ref<DatabaseStatement> _prepareStatement(const StringParam& _sql) override
				{
					StringCstr sql(_sql);
					ObjectLocker lock(this);
					Ref<DatabaseStatement> ret;
					sqlite3_stmt* statement = sl_null;
					if (SQLITE_OK == sqlite3_prepare_v2(m_db, sql.getData(), -1, &statement, sl_null)) {
						ret = new StatementImpl(this, m_db, statement);
						if (ret.isNotNull()) {
							return ret;
						}
						sqlite3_finalize(statement);
					}
					return ret;
				}

				String getErrorMessage() override
				{
					String error = String::create(sqlite3_errmsg(m_db));
					if (error.isEmpty() || error == "not an error") {
						return sl_null;
					}
					return error;
				}

				sl_bool isDatabaseExisting(const StringParam& name) override
				{
					return sl_false;
				}
				
				List<String> getDatabases() override
				{
					return sl_null;
				}

				sl_bool isTableExisting(const StringParam& _name) override
				{
					SqlBuilder builder(m_dialect);
					SLIB_STATIC_STRING(s, "SELECT COUNT(*) FROM sqlite_master WHERE type='table' AND name LIKE ")
					builder.append(s);
					StringData name(_name);
					builder.appendIdentifier(name.getData(), name.getLength());
					return getValue(builder.toString()).getUint32() > 0;
				}
				
				List<String> getTables() override
				{
					List<String> ret;
					Ref<DatabaseCursor> cursor = query("SELECT name FROM sqlite_master WHERE type='table'");
					if (cursor.isNotNull()) {
						while (cursor->moveNext()) {
							ret.add_NoLock(cursor->getString(0));
						}
					}
					return ret;
				}

				sl_uint64 getLastInsertRowId() override
				{
					return (sl_uint64)(sqlite3_last_insert_rowid(m_db));
				}
				
			};

		}
	}

	Ref<SQLiteDatabase> SQLiteDatabase::open(const SQLiteParam& param)
	{
		return priv::sqlite::DatabaseImpl::open(param);
	}

	Ref<SQLiteDatabase> SQLiteDatabase::open(const String& path)
	{
		SQLiteParam param;
		param.path = path;
		return priv::sqlite::DatabaseImpl::open(param);
	}

}
