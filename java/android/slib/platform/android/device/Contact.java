package slib.platform.android.device;

import android.app.Activity;
import android.content.ContentProviderOperation;
import android.content.ContentResolver;
import android.database.Cursor;
import android.provider.ContactsContract;

import java.util.ArrayList;
import java.util.Vector;

import slib.platform.android.Logger;

public class Contact {

	public String namePrefix;
	public String givenName;
	public String middleName;
	public String familyName;
	public String nameSuffix;
	public String displayName;
	public String nickname;

	public String phoneNumbers;
	public String emails;


	private static String getStringValue(Cursor cur, String field) {
		int index = cur.getColumnIndex(field);
		if (index >= 0) {
			return cur.getString(index);
		}
		return null;
	}

	private static int getIntValue(Cursor cur, String field) {
		int index = cur.getColumnIndex(field);
		if (index >= 0) {
			return cur.getInt(index);
		}
		return -1;
	}

	private static void fillNamesToContact(ContentResolver resolver, String id, Contact contact) {
		try {
			Cursor cur = resolver.query(ContactsContract.Data.CONTENT_URI, null, ContactsContract.Data.MIMETYPE + "=? AND " + ContactsContract.Data.CONTACT_ID + "=?", new String[] {ContactsContract.CommonDataKinds.StructuredName.CONTENT_ITEM_TYPE, id}, null);
			if (cur != null) {
				if (cur.moveToNext()) {
					contact.namePrefix = getStringValue(cur, ContactsContract.CommonDataKinds.StructuredName.PREFIX);
					contact.givenName = getStringValue(cur, ContactsContract.CommonDataKinds.StructuredName.GIVEN_NAME);
					contact.middleName = getStringValue(cur, ContactsContract.CommonDataKinds.StructuredName.MIDDLE_NAME);
					contact.familyName = getStringValue(cur, ContactsContract.CommonDataKinds.StructuredName.FAMILY_NAME);
					contact.nameSuffix = getStringValue(cur, ContactsContract.CommonDataKinds.StructuredName.SUFFIX);
				}
				cur.close();
			}
		} catch (Exception e) {
			Logger.exception(e);
		}
	}

	private static String getPhoneLabel(int type) {
		switch (type) {
			case ContactsContract.CommonDataKinds.Phone.TYPE_HOME:
				return "Home";
			case ContactsContract.CommonDataKinds.Phone.TYPE_MOBILE:
				return "Mobile";
			case ContactsContract.CommonDataKinds.Phone.TYPE_WORK:
				return "Work";
			case ContactsContract.CommonDataKinds.Phone.TYPE_FAX_WORK:
				return "WorkFax";
			case ContactsContract.CommonDataKinds.Phone.TYPE_FAX_HOME:
				return "HomeFax";
			case ContactsContract.CommonDataKinds.Phone.TYPE_PAGER:
				return "Pager";
			case ContactsContract.CommonDataKinds.Phone.TYPE_OTHER:
				return "Other";
			case ContactsContract.CommonDataKinds.Phone.TYPE_CALLBACK:
				return "Callback";
			case ContactsContract.CommonDataKinds.Phone.TYPE_CAR:
				return "Car";
			case ContactsContract.CommonDataKinds.Phone.TYPE_COMPANY_MAIN:
				return "Company";
			case ContactsContract.CommonDataKinds.Phone.TYPE_ISDN:
				return "ISDN";
			case ContactsContract.CommonDataKinds.Phone.TYPE_MAIN:
				return "Main";
			case ContactsContract.CommonDataKinds.Phone.TYPE_OTHER_FAX:
				return "Fax";
			case ContactsContract.CommonDataKinds.Phone.TYPE_RADIO:
				return "Radio";
			case ContactsContract.CommonDataKinds.Phone.TYPE_TELEX:
				return "Telex";
			case ContactsContract.CommonDataKinds.Phone.TYPE_TTY_TDD:
				return "TTY";
			case ContactsContract.CommonDataKinds.Phone.TYPE_WORK_MOBILE:
				return "WorkMobile";
			case ContactsContract.CommonDataKinds.Phone.TYPE_WORK_PAGER:
				return "WorkPager";
			case ContactsContract.CommonDataKinds.Phone.TYPE_ASSISTANT:
				return "Assistant";
			case ContactsContract.CommonDataKinds.Phone.TYPE_MMS:
				return "MMS";
		}
		return "";
	}

	private static void fillPhoneNumbersToContact(ContentResolver resolver, String id, Contact contact) {
		try {
			Cursor cur = resolver.query(ContactsContract.CommonDataKinds.Phone.CONTENT_URI, null, ContactsContract.Data.CONTACT_ID + "=?", new String[] {id}, null);
			if (cur != null) {
				StringBuilder buf = new StringBuilder();
				while (cur.moveToNext()) {
					String value = getStringValue(cur, ContactsContract.CommonDataKinds.Phone.NUMBER);
					if (value != null && value.length() > 0) {
						String label = getPhoneLabel(getIntValue(cur, ContactsContract.CommonDataKinds.Phone.TYPE));
						if (buf.length() > 0) {
							buf.append(",");
						}
						buf.append(label);
						buf.append(":");
						buf.append(value);
					}
				}
				cur.close();
				contact.phoneNumbers = buf.toString();
			}
		} catch (Exception e) {
			Logger.exception(e);
		}
	}

	private static String getEmailLabel(int type) {
		switch (type) {
			case ContactsContract.CommonDataKinds.Email.TYPE_HOME:
				return "Home";
			case ContactsContract.CommonDataKinds.Email.TYPE_WORK:
				return "Work";
			case ContactsContract.CommonDataKinds.Email.TYPE_OTHER:
				return "Other";
			case ContactsContract.CommonDataKinds.Email.TYPE_MOBILE:
				return "Mobile";
		}
		return "";
	}

	private static void fillEmailsToContact(ContentResolver resolver, String id, Contact contact) {
		try {
			Cursor cur = resolver.query(ContactsContract.CommonDataKinds.Email.CONTENT_URI, null, ContactsContract.Data.CONTACT_ID + "=?", new String[] {id}, null);
			if (cur != null) {
				StringBuilder buf = new StringBuilder();
				while (cur.moveToNext()) {
					String value = getStringValue(cur, ContactsContract.CommonDataKinds.Email.ADDRESS);
					if (value != null && value.length() > 0) {
						String label = getEmailLabel(getIntValue(cur, ContactsContract.CommonDataKinds.Email.TYPE));
						if (buf.length() > 0) {
							buf.append(",");
						}
						buf.append(label);
						buf.append(":");
						buf.append(value);
					}
				}
				cur.close();
				contact.emails = buf.toString();
			}
		} catch (Exception e) {
			Logger.exception(e);
		}
	}

	public static Contact[] getAllContacts(Activity activity) {
		try {
			ContentResolver resolver = activity.getContentResolver();
			if (resolver != null) {
				Cursor cur = resolver.query(ContactsContract.Contacts.CONTENT_URI, null, null, null, null);
				if (cur != null) {
					Vector<Contact> contacts = null;
					int fieldId = cur.getColumnIndex(ContactsContract.Contacts._ID);
					int fieldName = cur.getColumnIndex(ContactsContract.Contacts.DISPLAY_NAME);
					if (fieldId >= 0 && cur.getCount() > 0) {
						contacts = new Vector<Contact>();
						while (cur.moveToNext()) {
							String id = cur.getString(fieldId);
							if (id != null) {
								Contact contact = new Contact();
								if (fieldName >= 0) {
									contact.displayName = cur.getString(fieldName);
								}
								fillNamesToContact(resolver, id, contact);
								fillPhoneNumbersToContact(resolver, id, contact);
								fillEmailsToContact(resolver, id, contact);
								contacts.add(contact);
							}
						}
					}
					cur.close();
					if (contacts != null) {
						return contacts.toArray(new Contact[] {});
					}
				}
			}

		} catch (Exception e) {
			Logger.exception(e);
		}
		return new Contact[] {};
	}

	public static boolean addContact(Activity activity, Contact contact) {
		try {
			long rawContactId = 0;
			ArrayList<ContentProviderOperation> ops = new ArrayList<ContentProviderOperation>();
			ContentProviderOperation.Builder builder = ContentProviderOperation.newInsert(ContactsContract.Data.CONTENT_URI)
					.withValue(ContactsContract.Data.RAW_CONTACT_ID, rawContactId)
					.withValue(ContactsContract.Data.MIMETYPE, ContactsContract.CommonDataKinds.StructuredName.CONTENT_ITEM_TYPE);
			if (contact.namePrefix != null && contact.namePrefix.length() > 0) {
				builder = builder.withValue(ContactsContract.CommonDataKinds.StructuredName.PREFIX, contact.namePrefix);
			}
			if (contact.nameSuffix != null && contact.nameSuffix.length() > 0) {
				builder = builder.withValue(ContactsContract.CommonDataKinds.StructuredName.SUFFIX, contact.nameSuffix);
			}
			if (contact.familyName != null && contact.familyName.length() > 0) {
				builder = builder.withValue(ContactsContract.CommonDataKinds.StructuredName.FAMILY_NAME, contact.familyName);
			}
			if (contact.givenName != null && contact.givenName.length() > 0) {
				builder = builder.withValue(ContactsContract.CommonDataKinds.StructuredName.GIVEN_NAME, contact.givenName);
			}
			if (contact.middleName != null && contact.middleName.length() > 0) {
				builder = builder.withValue(ContactsContract.CommonDataKinds.StructuredName.MIDDLE_NAME, contact.middleName);
			}
			if (contact.nickname != null && contact.nickname.length() > 0) {
				builder = builder.withValue(ContactsContract.CommonDataKinds.StructuredName.DISPLAY_NAME, contact.nickname);
			}
			ops.add(builder.build());
			if (contact.phoneNumbers != null) {
				String[] arr = contact.phoneNumbers.split(",");
				for (String phoneNumber : arr) {
					ops.add(ContentProviderOperation.newInsert(ContactsContract.Data.CONTENT_URI)
							.withValue(ContactsContract.Data.RAW_CONTACT_ID, rawContactId)
							.withValue(ContactsContract.Data.MIMETYPE, ContactsContract.CommonDataKinds.Phone.CONTENT_ITEM_TYPE)
							.withValue(ContactsContract.CommonDataKinds.Phone.NUMBER, phoneNumber)
							.withValue(ContactsContract.CommonDataKinds.Phone.TYPE, ContactsContract.CommonDataKinds.Phone.TYPE_CUSTOM)
							.build());
				}
			}
			if (contact.emails != null) {
				String[] arr = contact.emails.split(",");
				for (String email : arr) {
					ops.add(ContentProviderOperation.newInsert(ContactsContract.Data.CONTENT_URI)
							.withValue(ContactsContract.Data.RAW_CONTACT_ID, rawContactId)
							.withValue(ContactsContract.Data.MIMETYPE, ContactsContract.CommonDataKinds.Email.CONTENT_ITEM_TYPE)
							.withValue(ContactsContract.CommonDataKinds.Email.ADDRESS, email)
							.withValue(ContactsContract.CommonDataKinds.Email.TYPE, ContactsContract.CommonDataKinds.Email.TYPE_CUSTOM)
							.build());
				}
			}
			activity.getContentResolver().applyBatch(ContactsContract.AUTHORITY, ops);
			return true;
		} catch (Exception e) {
			Logger.exception(e);
		}
		return false;
	}

}
