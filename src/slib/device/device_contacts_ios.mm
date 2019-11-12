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

#include "slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_IOS)

#include "slib/device/device.h"

#include "slib/social/contact.h"
#include "slib/core/platform_apple.h"
#include "slib/core/safe_static.h"

#import <Contacts/Contacts.h>

namespace slib
{

	namespace priv
	{
		namespace device_contact
		{
			
			class SharedContext
			{
			public:
				NSMutableDictionary* dictLabel;
				
			public:
				SharedContext()
				{
					dictLabel = [NSMutableDictionary new];
					
					addMapping(CNLabelHome, @"Home");
					addMapping(CNLabelWork, @"Work");
					if (@available(iOS 13.0, *)) {
						addMapping(CNLabelSchool, @"School");
					}
					addMapping(CNLabelOther, @"Other");

					addMapping(CNLabelPhoneNumberMain, @"Main");
					addMapping(CNLabelPhoneNumberiPhone, @"iPhone");
					addMapping(CNLabelPhoneNumberMobile, @"Mobile");
					addMapping(CNLabelPhoneNumberHomeFax, @"HomeFax");
					addMapping(CNLabelPhoneNumberWorkFax, @"WorkFax");
					addMapping(CNLabelPhoneNumberOtherFax, @"Fax");
					addMapping(CNLabelPhoneNumberPager, @"Pager");

					addMapping(CNLabelEmailiCloud, @"iCloud");

					addMapping(CNLabelURLAddressHomePage, @"HomePage");
										
					addMapping(CNLabelDateAnniversary, @"Anniversary");
				}
				
			public:
				void addMapping(NSString* s1, NSString* s2)
				{
					[dictLabel setObject:s2 forKey:s1];
				}
				
			};
		
			SLIB_SAFE_STATIC_GETTER(SharedContext, GetSharedContext)
			
			static String GetLabel(NSString* label)
			{
				SharedContext* context = GetSharedContext();
				NSString* s = [context->dictLabel objectForKey:label];
				return Apple::getStringFromNSString(s);
			}
		}
	}

	using namespace priv::device_contact;

	List<Contact> Device::getAllContacts()
	{
		CNContactStore* store = [CNContactStore new];
		if (store != nil) {
			NSArray* containers = [store containersMatchingPredicate:nil error:nil];
			if (containers != nil) {
				List<Contact> ret;
				NSArray* fields = @[[CNContactFormatter descriptorForRequiredKeysForStyle:CNContactFormatterStyleFullName], CNContactEmailAddressesKey, CNContactPhoneNumbersKey];
				for (CNContainer* container in containers) {
					NSPredicate* predicate = [CNContact predicateForContactsInContainerWithIdentifier:container.identifier];
					if (predicate != nil) {
						NSArray* contacts = [store unifiedContactsMatchingPredicate:predicate keysToFetch:fields error:nil];
						if (contacts != nil) {
							for (CNContact* contact in contacts) {
								Contact c;
								c.namePrefix = Apple::getStringFromNSString(contact.namePrefix);
								c.givenName = Apple::getStringFromNSString(contact.givenName);
								c.middleName = Apple::getStringFromNSString(contact.middleName);
								c.familyName = Apple::getStringFromNSString(contact.familyName);
								c.nameSuffix = Apple::getStringFromNSString(contact.nameSuffix);
								c.nickname = Apple::getStringFromNSString(contact.nickname);
								
								NSArray* phoneNumbers = contact.phoneNumbers;
								if (phoneNumbers != nil) {
									for (CNLabeledValue<CNPhoneNumber*>* phoneNumber : phoneNumbers) {
										CNPhoneNumber* pn = phoneNumber.value;
										if (pn != nil) {
											String strValue = Apple::getStringFromNSString(pn.stringValue);
											if (strValue.isNotEmpty()) {
												c.phoneNumbers.add_NoLock(GetLabel(phoneNumber.label), Move(strValue));
											}
										}
									}
								}
								NSArray* emails = contact.emailAddresses;
								if (emails != nil) {
									for (CNLabeledValue<NSString*>* email : emails) {
										String strValue = Apple::getStringFromNSString(email.value);
										if (strValue.isNotEmpty()) {
											c.emails.add_NoLock(GetLabel(email.label), Move(strValue));
										}
									}
								}
								ret.add_NoLock(c);
							}
						}
					}
				}
				return ret;
			}
		}
		return sl_null;
	}

}

#endif
