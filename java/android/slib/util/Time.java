package slib.util;

import java.text.DateFormatSymbols;
import java.util.Calendar;
import java.util.Vector;

public class Time {

	public static interface LocaleProvider {
		String getShortWeekday(int day);

		String getLongWeekday(int day);
	}

	static class DefaultLocaleProvider implements LocaleProvider {

		@Override
		public String getLongWeekday(int day) {
			DateFormatSymbols symbols = new DateFormatSymbols();
			return symbols.getWeekdays()[day + 1];
		}

		@Override
		public String getShortWeekday(int day) {
			DateFormatSymbols symbols = new DateFormatSymbols();
			return symbols.getShortWeekdays()[day + 1];
		}
	}

	public static LocaleProvider defaultLocaleProvider = new DefaultLocaleProvider();

	public static Calendar getNowCalendar() {
		return Calendar.getInstance(java.util.TimeZone.getDefault(), java.util.Locale.getDefault());
	}

	Calendar calendar;
	LocaleProvider locale;

	public Time() {
		this.calendar = getNowCalendar();
		this.locale = defaultLocaleProvider;
	}

	public Time(Calendar calendar) {
		if (calendar == null) {
			calendar = getNowCalendar();
		}
		this.calendar = calendar;
		this.locale = defaultLocaleProvider;
	}

	public Time(int year, int month, int date) {
		this.calendar = getNowCalendar();
		this.locale = defaultLocaleProvider;
		set(year, month, date, 0, 0, 0);
	}

	public Time(int year, int month, int date, int hour, int minute, int second) {
		this.calendar = getNowCalendar();
		this.locale = defaultLocaleProvider;
		set(year, month, date, hour, minute, second);
	}

	public Time(long time) {
		this.calendar = getNowCalendar();
		this.locale = defaultLocaleProvider;
		calendar.setTimeInMillis(time);
	}

	public Time(String str) {
		this.calendar = getNowCalendar();
		this.locale = defaultLocaleProvider;
		if (str != null && str.length() > 0) {
			int[] arr = parseTime(str);
			set(arr[0], arr[1], arr[2], arr[3], arr[4], arr[5]);
		}
	}

	public Time duplicate() {
		return new Time((Calendar) calendar.clone());
	}

	public void set(int year, int month, int date, int hour, int minute, int second) {
		calendar.set(Calendar.YEAR, year);
		calendar.set(Calendar.MONTH, month - 1);
		calendar.set(Calendar.DATE, date);
		calendar.set(Calendar.HOUR_OF_DAY, hour);
		calendar.set(Calendar.MINUTE, minute);
		calendar.set(Calendar.SECOND, second);
	}

	public int getYear() {
		return calendar.get(Calendar.YEAR);
	}

	public void setYear(int year) {
		calendar.set(Calendar.YEAR, year);
	}

	public Time addYear(int year) {
		Time ret = duplicate();
		ret.calendar.add(Calendar.YEAR, year);
		return ret;
	}

	public int getMonth() {
		return calendar.get(Calendar.MONTH) + 1;
	}

	public void setMonth(int month) {
		calendar.set(Calendar.MONTH, month - 1);
	}

	public Time addMonth(int month) {
		Time ret = duplicate();
		ret.calendar.add(Calendar.MONTH, month);
		return ret;
	}

	public int getQuarter() {
		return (getMonth() - 1) / 3 + 1;
	}

	public int getDate() {
		return calendar.get(Calendar.DATE);
	}

	public void setDate(int date) {
		calendar.set(Calendar.DATE, date);
	}

	public Time addDate(int date) {
		Time ret = duplicate();
		ret.calendar.add(Calendar.DATE, date);
		return ret;
	}

	public Time addDay(int day) {
		Time ret = duplicate();
		ret.calendar.add(Calendar.DATE, day);
		return ret;
	}

	public int getHour() {
		return calendar.get(Calendar.HOUR_OF_DAY);
	}

	public static void main() {
		System.out.println(new Time().toString());
	}

	public void setHour(int hour) {
		calendar.set(Calendar.HOUR_OF_DAY, hour);
	}

	public Time addHour(int hour) {
		Time ret = duplicate();
		ret.calendar.add(Calendar.HOUR_OF_DAY, hour);
		return ret;
	}

	public int getMinute() {
		return calendar.get(Calendar.MINUTE);
	}

	public void setMinute(int minute) {
		calendar.set(Calendar.MINUTE, minute);
	}

	public Time addMinute(int minute) {
		Time ret = duplicate();
		ret.calendar.add(Calendar.MINUTE, minute);
		return ret;
	}

	public int getSecond() {
		return calendar.get(Calendar.SECOND);
	}

	public void setSecond(int second) {
		calendar.set(Calendar.SECOND, second);
	}

	public Time addSecond(int second) {
		Time ret = duplicate();
		ret.calendar.add(Calendar.SECOND, second);
		return ret;
	}

	public int getMillisecond() {
		return calendar.get(Calendar.MILLISECOND);
	}

	public void setMillisecond(int milli) {
		calendar.set(Calendar.MILLISECOND, milli);
	}

	public Time addMillisecond(int millisecond) {
		Time ret = duplicate();
		ret.calendar.add(Calendar.MILLISECOND, millisecond);
		return ret;
	}

	public long getTimeInMilliseconds() {
		return calendar.getTimeInMillis();
	}
	
	public static long getCurrentTimeInMilliseconds() {
		return (new Time()).getTimeInMilliseconds();
	}

	public void setTimeInMilliseconds(long millis) {
		calendar.setTimeInMillis(millis);
	}

	public int getDay() {
		return calendar.get(Calendar.DAY_OF_WEEK);
	}

	public void setDay(int day) {
		calendar.set(Calendar.DAY_OF_WEEK, day);
	}

	public String getWeekday() {
		return locale.getLongWeekday(getDay() - 1);
	}

	public String getShortWeekday() {
		return locale.getShortWeekday(getDay() - 1);
	}

	public int getDayOfYear() {
		return calendar.get(Calendar.DAY_OF_YEAR);
	}

	public void setDayOfYear(int day) {
		calendar.set(Calendar.DAY_OF_YEAR, day);
	}

	public int getWeekOfYear() {
		return calendar.get(Calendar.WEEK_OF_YEAR);
	}

	public void setWeekOfYear(int week) {
		calendar.set(Calendar.WEEK_OF_YEAR, week);
	}

	public int getWeekOfMonth() {
		return calendar.get(Calendar.WEEK_OF_MONTH);
	}

	public void setWeekOfMonth(int week) {
		calendar.set(Calendar.WEEK_OF_MONTH, week);
	}

	public int getDaysInMonth() {
		return calendar.getActualMaximum(Calendar.DAY_OF_MONTH);
	}

	public int getDaysInYear() {
		return calendar.getActualMaximum(Calendar.DAY_OF_YEAR);
	}

	public String toString() {

		String ret = NumberFormator.formatInt(getYear(), 4) + "-" + NumberFormator.formatInt(getMonth(), 2) + "-"
				+ NumberFormator.formatInt(getDate(), 2) + " " + NumberFormator.formatInt(getHour(), 2) + ":"
				+ NumberFormator.formatInt(getMinute(), 2) + ":" + NumberFormator.formatInt(getSecond(), 2);

		return ret;
	}

	public String format(String format) {
		String str = format;
		str = str.replaceAll("%y", String.valueOf(getYear()));
		str = str.replaceAll("%Y", NumberFormator.formatInt(getYear(), 4));
		str = str.replaceAll("%m", String.valueOf(getMonth()));
		str = str.replaceAll("%M", NumberFormator.formatInt(getMonth(), 2));
		str = str.replaceAll("%d", String.valueOf(getDate()));
		str = str.replaceAll("%D", NumberFormator.formatInt(getDate(), 2));
		str = str.replaceAll("%w", getShortWeekday());
		str = str.replaceAll("%W", getWeekday());
		str = str.replaceAll("%h", String.valueOf(getHour()));
		str = str.replaceAll("%H", NumberFormator.formatInt(getHour(), 2));
		str = str.replaceAll("%i", String.valueOf(getMinute()));
		str = str.replaceAll("%I", NumberFormator.formatInt(getMinute(), 2));
		str = str.replaceAll("%s", String.valueOf(getSecond()));
		str = str.replaceAll("%S", NumberFormator.formatInt(getSecond(), 2));
		str = str.replaceAll("%n", String.valueOf(getMillisecond()));
		return str;
	}

	public String getDateString() {
		String ret = NumberFormator.formatInt(getYear(), 4) + "-" + NumberFormator.formatInt(getMonth(), 2) + "-"
				+ NumberFormator.formatInt(getDate(), 2);
		return ret;
	}

	public String getDateString1() {
		String ret = NumberFormator.formatInt(getYear(), 4) + "/" + getMonth() + "/" + getDate();
		return ret;
	}
	
	static String arrMonth[] = {"JAN", "FEB", "MAR", "APR", "MAY", "JUN", "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"};
	public String getDateString2() {
		String ret = arrMonth[getMonth()-1] + " " + getDate() + " " + getYear();
		return ret;
	}


	public String getTimeString() {
		String ret = NumberFormator.formatInt(getHour(), 2) + ":" + NumberFormator.formatInt(getMinute(), 2) + ":"
				+ NumberFormator.formatInt(getSecond(), 2);
		return ret;
	}

	static int[] parseTime(String date) {
		Vector<Integer> v = new Vector<Integer>();
		char ch[] = date.toCharArray();
		int len = ch.length;
		int i = 0;
		while (i < len) {
			int value = 0;
			while (i < len) {
				if (ch[i] >= '0' && ch[i] <= '9') {
					value = value * 10 + (ch[i] - '0');
				} else {
					break;
				}
				i++;
			}
			while (i < len && (ch[i] < '0' || ch[i] > '9')) {
				if (ch[i] == ':') {
					int start = v.size();
					for (int j = start; j < 3; j++) {
						v.add(0);
					}
				}
				i++;
			}
			v.add(value);
		}
		int[] ret = new int[6];
		for (i = 0; i < v.size(); i++) {
			if (i >= 6)
				break;
			ret[i] = v.get(i);
		}
		for (; i < 6; i++)
			ret[i] = 0;
		return ret;
	}

	public String[] getLongWeekdays() {
		String[] ret = new String[7];
		for (int i = 0; i < 7; i++) {
			ret[i] = locale.getLongWeekday(i);
		}
		return ret;
	}

	public String[] getShortWeekdays() {
		String[] ret = new String[7];
		for (int i = 0; i < 7; i++) {
			ret[i] = locale.getShortWeekday(i);
		}
		return ret;
	}

}
