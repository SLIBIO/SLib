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

#include "ui_text_ios.h"
/*
@interface SLIBTextInput : UIResponder<UITextInput>
{
	@public slib::Ref<slib::TextInput> m_textInput;
}
@end

@interface SLIBTextRange : UITextRange
{
	@public slib::sl_text_pos location;
	@public slib::sl_text_pos length;
}
@end

namespace slib
{
		
	namespace priv
	{
		namespace ui_text
		{

			SLIB_INLINE static UITextRange* ToNative(const TextRange& range)
			{
				SLIBTextRange* ret = [[SLIBTextRange alloc] init];
				if (ret != nil) {
					ret->location = range.location;
					ret->length = range.length;
					return ret;
				}
				return nil;
			}
			
			SLIB_INLINE static TextRange FromNative(UITextRange* _range)
			{
				if (!_range) {
					return TextRange();
				}
				SLIBTextRange* range = (SLIBTextRange*)_range;
				return TextRange(range->location, range->length);
			}
			
		}
	}

	UIResponder* iOS_TextInput::getNativeInstance(TextInput* input)
	{
		if (!input) {
			return nil;
		}
		SLIBTextInput* instance = [[SLIBTextInput alloc] init];
		if (instance != nil) {
			instance->m_textInput = input;
		}
		return instance;
	}

}

@implementation SLIBTextInput

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
	return slib::ToNative(m_textInput->getSelectedRange());
}

- (void)setSelectedTextRange:(UITextRange*)selectedTextRange
{
	m_textInput->setSelectedRange(slib::FromNative(selectedTextRange));
}

- (NSString*)textInRange:(UITextRange*)range
{
	return slib::Apple::getNSStringFromString(m_textInput->getTextInRange(slib::FromNative(range)));
}

- (void)replaceRange:(UITextRange*)range withText:(NSString*)text
{
	m_textInput->replaceText(slib::FromNative(range), slib::Apple::getStringFromNSString(text));
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
*/
