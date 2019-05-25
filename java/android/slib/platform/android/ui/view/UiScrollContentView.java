package slib.platform.android.ui.view;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Rect;
import android.view.View;
import android.widget.AbsoluteLayout;

public class UiScrollContentView extends UiGroupView {

	int mScrollX;
	int mScrollY;
	int mScrollWidth;
	int mScrollHeight;

	public UiScrollContentView(Context context) {
		super(context);
	}

	public void setupTiles(IView scrollView) {
		Rect frame = scrollView.getUIFrame();
		View view = (View)(scrollView);
		setupTiles(view.getScrollX(), view.getScrollY(), frame.width(), frame.height());
	}

	public void setupTiles(int scrollX, int scrollY, int width, int height) {
		mScrollX = scrollX;
		mScrollY = scrollY;
		mScrollWidth = width;
		mScrollHeight = height;
		if (width <= 0 || height <= 0) {
			return;
		}
		Rect frameContent = getUIFrame();
		int widthContent = frameContent.width();
		int heightContent = frameContent.height();
		int maxScrollX = widthContent - width;
		int maxScrollY = heightContent - height;
		if (scrollX > maxScrollX) {
			scrollX = maxScrollX;
		}
		if (scrollX < 0) {
			scrollX = 0;
		}
		if (scrollY > maxScrollY) {
			scrollY = maxScrollY;
		}
		if (scrollY < 0) {
			scrollY = 0;
		}
		int col = scrollX / width;
		int row = scrollY / height;
		setupTile(col, row);
		boolean flagExtendX = scrollX % width != 0;
		boolean flagExtendY = scrollY % height != 0;
		if (flagExtendX) {
			setupTile(col + 1, row);
		} else {
			hideTile(col + 1, row);
		}
		if (flagExtendY) {
			setupTile(col, row + 1);
		} else {
			hideTile(col, row + 1);
		}
		if (flagExtendX && flagExtendY) {
			setupTile(col + 1, row + 1);
		} else {
			hideTile(col + 1, row + 1);
		}
	}

	Tile[] mTiles = new Tile[4];

	private int getTileIndex(int col, int row) {
		return (col % 2) * 2 + (row % 2);
	}

	private void setupTile(int col, int row) {
		int index = getTileIndex(col, row);
		Tile tile = mTiles[index];
		int width = mScrollWidth;
		int height = mScrollHeight;
		boolean flagNew = false;
		boolean flagInvalidate = false;
		if (tile == null) {
			flagNew = true;
			tile = new Tile(getContext());
			AbsoluteLayout.LayoutParams params = new AbsoluteLayout.LayoutParams(width, height, 0, 0);
			tile.setLayoutParams(params);
			addView(tile);
			mTiles[index] = tile;
		} else {
			AbsoluteLayout.LayoutParams params = (AbsoluteLayout.LayoutParams)(tile.getLayoutParams());
			if (params.width != width || params.height != height) {
				params.width = width;
				params.height = height;
				tile.setLayoutParams(params);
				flagInvalidate = true;
			}
		}
		float x = (float)(col * width);
		float y = (float)(row * height);
		if (tile.getTranslationX() != x) {
			tile.setTranslationX(x);
			flagInvalidate = true;
		}
		if (tile.getTranslationY() != y) {
			tile.setTranslationY(y);
			flagInvalidate = true;
		}
		if (tile.getVisibility() != View.VISIBLE) {
			tile.setVisibility(View.VISIBLE);
			flagInvalidate = true;
		}
		if (!flagNew && flagInvalidate) {
			tile.invalidate();
		}
	}

	private void hideTile(int col, int row) {
		int index = getTileIndex(col, row);
		Tile tile = mTiles[index];
		if (tile != null) {
			tile.setVisibility(View.INVISIBLE);
		}
	}

	@Override
	public void onDraw(Canvas canvas) {
	}

	void drawContent(Canvas canvas, int left, int top) {
		UiView.onEventDraw(this, canvas, left, top, left + mScrollWidth, top + mScrollHeight);
	}

	@Override
	protected void onLayout(boolean changed, int l, int t, int r, int b) {
		super.onLayout(changed, l, t, r, b);
		for (int i = 0; i < 4; i++) {
			Tile tile = mTiles[i];
			if (tile != null) {
				tile.layout(0, 0, mScrollWidth, mScrollHeight);
			}
		}
	}

	@Override
	public void invalidate() {
		for (int i = 0; i < 4; i++) {
			Tile tile = mTiles[i];
			if (tile != null && tile.getVisibility() == View.VISIBLE) {
				tile.invalidate();
			}
		}
	}

	@Override
	public void invalidate(int left, int top, int right, int bottom) {
		for (int i = 0; i < 4; i++) {
			Tile tile = mTiles[i];
			if (tile != null && tile.getVisibility() == View.VISIBLE) {
				int leftTile = (int)(tile.getTranslationX());
				int topTile = (int)(tile.getTranslationY());
				int rightTile = leftTile + mScrollWidth;
				int bottomTile = topTile + mScrollHeight;
				if (left <= rightTile && right >= leftTile && top <= bottomTile && bottom >= topTile) {
					tile.invalidate();
				}
			}
		}
	}

	private class Tile extends View {

		public Tile(Context context) {
			super(context);
		}

		@Override
		public void onDraw(Canvas canvas) {
			canvas.save();
			float tx = getTranslationX();
			float ty = getTranslationY();
			canvas.translate(-tx, -ty);
			drawContent(canvas, (int)tx, (int)ty);
			canvas.restore();
		}

	}

}
