/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ui_text_ios.h"

@interface _priv_Slib_iOS_TextInput : UIResponder<UITextInput>
{
	@public slib::Ref<slib::TextInput> m_textInput;
}
@end

@interface _priv_Slib_iOS_TextRange : UITextRange
{
	@public slib::sl_text_pos location;
	@public slib::sl_text_pos length;
}
@end

namespace slib
{
	
	UIResponder* iOS_TextInput::getNativeInstance(TextInput* input)
	{
		if (!input) {
			return nil;
		}
		_priv_Slib_iOS_TextInput* instance = [[_priv_Slib_iOS_TextInput alloc] init];
		if (instance != nil) {
			instance->m_textInput = input;
		}
		return instance;
	}
	
	SLIB_INLINE static UITextRange* _priv_TextRange_toNative(const TextRange& range)
	{
		_priv_Slib_iOS_TextRange* ret = [[_priv_Slib_iOS_TextRange alloc] init];
		if (ret != nil) {
			ret->location = range.location;
			ret->length = range.length;
			return ret;
		}
		return nil;
	}
	
	SLIB_INLINE static TextRange _priv_TextRange_fromNative(UITextRange* _range)
	{
		if (!_range) {
			return TextRange();
		}
		_priv_Slib_iOS_TextRange* range = (_priv_Slib_iOS_TextRange*)_range;
		return TextRange(range->location, range->length);
	}
	
}

@implementation _priv_Slib_iOS_TextInput

- (BOOL)canBecomeFirstResponder {
	return YES;
}

@synthesize tokenizer = _tokenizer;

- (id<UITextInputTokenizer>)tokenizer
{
	if (_tokenizer == nil) {
		_tokenizer = [[UITextInputStringTokenizer alloc] initWithTextInput:self];
	}
	return _tokenizer;
}

- (UITextRange*)selectedTextRange
{
	return slib::_priv_TextRange_toNative(m_textInput->getSelectedRange());
}

- (void)setSelectedTextRange:(UITextRange*)selectedTextRange
{
	m_textInput->setSelectedRange(slib::_priv_TextRange_fromNative(selectedTextRange));
}

- (NSString*)textInRange:(UITextRange*)range
{
	return slib::Apple::getNSStringFromString(m_textInput->getTextInRange(slib::_priv_TextRange_fromNative(range)));
}

- (void)replaceRange:(UITextRange*)range withText:(NSString*)text
{
	m_textInput->replaceText(slib::_priv_TextRange_fromNative(range), slib::Apple::getStringFromNSString(text));
}

- (UITextWritingDirection)baseWritingDirectionForPosition:(nonnull UITextPosition *)position inDirection:(UITextStorageDirection)direction
{
	return UITextWritingDirectionNatural;
}

- (CGRect)caretRectForPosition:(nonnull UITextPosition *)position
{
	return CGRectMake(0, 0, 0, 0);
}

- (nullable UITextRange *)characterRangeAtPoint:(CGPoint)point
{
	return nil;
}

- (nullable UITextRange *)characterRangeByExtendingPosition:(nonnull UITextPosition *)position inDirection:(UITextLayoutDirection)direction
{
	return nil;
}


- (nullable UITextPosition *)closestPositionToPoint:(CGPoint)point
{
	return nil;
}


- (nullable UITextPosition *)closestPositionToPoint:(CGPoint)point withinRange:(nonnull UITextRange *)range
{
	return nil;
}

- (NSComparisonResult)comparePosition:(nonnull UITextPosition *)position toPosition:(nonnull UITextPosition *)other
{
	return NSOrderedAscending;
}

- (CGRect)firstRectForRange:(nonnull UITextRange *)range
{
	return CGRectMake(0, 0, 0, 0);
}

- (NSInteger)offsetFromPosition:(nonnull UITextPosition *)from toPosition:(nonnull UITextPosition *)toPosition
{
	return 0;
}

- (nullable UITextPosition *)positionFromPosition:(nonnull UITextPosition *)position inDirection:(UITextLayoutDirection)direction offset:(NSInteger)offset
{
	return nil;
}

- (nullable UITextPosition *)positionFromPosition:(nonnull UITextPosition *)position offset:(NSInteger)offset
{
	return nil;
}

- (nullable UITextPosition *)positionWithinRange:(nonnull UITextRange *)range farthestInDirection:(UITextLayoutDirection)direction
{
	return nil;
}

- (nonnull NSArray *)selectionRectsForRange:(nonnull UITextRange *)range
{
	return nil;
}

- (void)setBaseWritingDirection:(UITextWritingDirection)writingDirection forRange:(nonnull UITextRange *)range
{
}

- (void)setMarkedText:(nullable NSString *)markedText selectedRange:(NSRange)selectedRange
{
}

- (nullable UITextRange *)textRangeFromPosition:(nonnull UITextPosition *)fromPosition toPosition:(nonnull UITextPosition *)toPosition
{
	return nil;
}

- (void)unmarkText
{
}

- (void)deleteBackward
{
}

- (void)insertText:(nonnull NSString *)text
{
}

@synthesize beginningOfDocument;

@synthesize endOfDocument;

@synthesize hasText;

@synthesize inputDelegate;

@synthesize markedTextRange;

@synthesize markedTextStyle;

@end
