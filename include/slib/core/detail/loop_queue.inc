/*
 *   Copyright (c) 2008-2018 SLIBIO <https://github.com/SLIBIO>
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

namespace slib
{
	
	template <class T>
	LoopQueue<T>::LoopQueue(sl_size size, sl_size latency) noexcept
	{
		m_first = 0;
		m_count = 0;
		m_latency = latency;
		m_data = NewHelper<T>::create(size);
		if (m_data) {
			m_size = size;
		} else {
			m_size = 0;
		}
	}

	template <class T>
	LoopQueue<T>::~LoopQueue() noexcept
	{
		if (m_data) {
			NewHelper<T>::free(m_data, m_size);
		}
	}

	template <class T>
	sl_size LoopQueue<T>::getQueueSize() const noexcept
	{
		return m_size;
	}

	template <class T>
	sl_bool LoopQueue<T>::setQueueSize(sl_size size) noexcept
	{
		ObjectLocker lock(this);
		if (m_data) {
			NewHelper<T>::free(m_data, m_size);
			m_data = sl_null;
		}
		m_data = NewHelper<T>::create(size);
		if (m_data) {
			m_first = 0;
			m_count = 0;
			m_size = size;
			return sl_true;
		}
		return sl_false;
	}

	template <class T>
	sl_size LoopQueue<T>::removeAll() noexcept
	{
		ObjectLocker lock(this);
		sl_size count = m_count;
		m_first = 0;
		m_count = 0;
		return count;
	}

	template <class T>
	T* LoopQueue<T>::getBuffer() const noexcept
	{
		return m_data;
	}

	template <class T>
	sl_size LoopQueue<T>::getCount() const noexcept
	{
		return m_count;
	}

	template <class T>
	void LoopQueue<T>::setLatency(sl_size latency) noexcept
	{
		m_latency = latency;
	}

	template <class T>
	sl_size LoopQueue<T>::getLatency() const noexcept
	{
		return m_latency;
	}

	template <class T>
	sl_bool LoopQueue<T>::push(const T& data, sl_bool flagShift) noexcept
	{
		ObjectLocker lock(this);
		if (m_size == 0) {
			return sl_false;
		}
		if (m_count == m_size && !flagShift) {
			return sl_false;
		}
		sl_size last = m_first + m_count;
		m_data[last % m_size] = data;
		m_count ++;
		if (m_count > m_size) {
			m_count = m_size;
			m_first = (last + 1) % m_size;
		}
		return sl_true;
	}

	template <class T>
	sl_bool LoopQueue<T>::push(const T* buffer, sl_size count, sl_bool flagShift) noexcept
	{
		ObjectLocker lock(this);
		if (m_size == 0) {
			return sl_false;
		}
		if (m_count + count > m_size && !flagShift) {
			return sl_false;
		}
		sl_size last = m_first + m_count;
		if (count > m_size) {
			buffer += (count - m_size);
			last += (count - m_size);
			m_count += (count - m_size);
			count = m_size;
		}
		sl_size i = last % m_size;
		sl_size n = 0;
		while (i < m_size && n < count) {
			m_data[i] = buffer[n];
			i ++;
			n ++;
		}
		i = 0;
		while (n < count) {
			m_data[i] = buffer[n];
			i ++;
			n ++;
		}
		m_count += count;
		if (m_count > m_size) {
			m_first = (m_first + m_count) % m_size;
			m_count = m_size;
		}
		return sl_true;
	}

	template <class T>
	sl_bool LoopQueue<T>::pop(T& output) noexcept
	{
		ObjectLocker lock(this);
		sl_bool ret = sl_false;
		if (m_count > m_latency) {
			output = m_data[m_first % m_size];
			m_first = (m_first + 1) % m_size;
			m_count --;
			ret = sl_true;
		}
		return ret;
	}

	template <class T>
	sl_bool LoopQueue<T>::pop(T* buffer, sl_size count) noexcept
	{
		ObjectLocker lock(this);
		sl_bool ret = sl_false;
		if (count <= m_count && m_count > m_latency) {
			sl_size n = 0;
			sl_size i = m_first;
			while (i < m_size && n < count) {
				buffer[n] = m_data[i];
				i ++;
				n ++;
			}
			i = 0;
			while (n < count) {
				buffer[n] = m_data[i];
				i ++;
				n ++;
			}
			m_first = (m_first + count) % m_size;
			m_count -= count;
			ret = sl_true;
		}
		return ret;
	}

	template <class T>
	sl_size LoopQueue<T>::copy(T* buffer, sl_size count) const noexcept
	{
		ObjectLocker lock(this);
		if (count > m_count) {
			count = m_count;
		}
		{
			sl_size n = 0;
			sl_size i = m_first;
			while (i < m_size && n < count) {
				buffer[n] = m_data[i];
				i++;
				n++;
			}
			i = 0;
			while (n < count) {
				buffer[n] = m_data[i];
				i++;
				n++;
			}
		}
		return count;
	}

}
