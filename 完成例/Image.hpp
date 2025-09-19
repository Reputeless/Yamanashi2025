#pragma once
#include <vector>		// std::vector
#include <cassert>		// assert
#include <algorithm>	// std::fill
#include <span>			// std::span
#include "Color.hpp"	// mini::Color
#include "Point.hpp"	// mini::Point

namespace mini
{
	/// @brief 画像データを表現するクラス
	class Image
	{
	public:

		/// @brief デフォルトコンストラクタ
		[[nodiscard]]
		Image() = default;

		/// @brief 指定したサイズの画像を作成します
		/// @param width 画像の幅（ピクセル）
		/// @param height 画像の高さ（ピクセル）
		/// @param fillColor 各ピクセルの初期色（デフォルトでは白）
		[[nodiscard]]
		Image(int width, int height, const Color& fillColor = Color{ 1.0 })
		{
			// サイズが不正な場合は空の画像を作成する
			if ((width <= 0) || (height <= 0))
			{
				return;
			}

			m_width = width;
			m_height = height;
			m_pixels.resize((width * height), fillColor);
		}

		[[nodiscard]]
		explicit Image(std::string_view fileName);

		/// @brief 画像の幅（ピクセル）を返します。
		/// @return 画像の幅（ピクセル）
		[[nodiscard]]
		int width() const noexcept
		{
			return m_width;
		}

		/// @brief 画像の高さ（ピクセル）を返します。
		/// @return 画像の高さ（ピクセル）
		[[nodiscard]]
		int height() const noexcept
		{
			return m_height;
		}

		/// @brief 画像のピクセル数を返します。
		/// @return 画像のピクセル数
		[[nodiscard]]
		int numPixels() const noexcept
		{
			return static_cast<int>(m_pixels.size());
		}

		/// @brief 画像が空であるかを返します。
		/// @return 画像が空である場合 true, それ以外の場合は false
		[[nodiscard]]
		bool isEmpty() const noexcept
		{
			return m_pixels.empty();
		}

		/// @brief 画像が空でないかを返します。
		/// @return 画像が空でない場合 true, それ以外の場合は false
		[[nodiscard]]
		explicit operator bool() const noexcept
		{
			return !isEmpty();
		}

		/// @brief 指定した位置が画像の範囲内であるかを返します。
		/// @param y 行番号
		/// @param x 列番号
		/// @return 指定した位置が画像の範囲内である場合 true, それ以外の場合は false
		[[nodiscard]]
		bool inBounds(int y, int x) const noexcept
		{
			return ((0 <= y) && (y < m_height) && (0 <= x) && (x < m_width));
		}

		/// @brief 指定した位置のピクセルの色を返します。範囲外の場合は黒を返します。
		/// @param y 行番号
		/// @param x 列番号
		/// @return 指定した位置のピクセルの色
		[[nodiscard]]
		Color getPixel(int y, int x) const noexcept
		{
			if (!inBounds(y, x))
			{
				return Color{ 0.0 }; // 範囲外の場合は黒を返す
			}

			return m_pixels[(y * m_width) + x];
		}

		/// @brief 指定した位置のピクセルの色を設定します。範囲外の場合は何もしません。
		/// @param y 行番号
		/// @param x 列番号
		/// @param color 設定する色
		void setPixel(int y, int x, const Color& color) noexcept
		{
			if (!inBounds(y, x))
			{
				return; // 範囲外の場合は何もしない
			}

			m_pixels[(y * m_width) + x] = color;
		}

		/// @brief y 行目の先頭ピクセルへのポインタを返します。
		/// @param y 行番号
		/// @return y 行目の先頭ピクセルへのポインタ
		[[nodiscard]]
		Color* operator [](int y) noexcept
		{
			assert((0 <= y) && (y < m_height));
			return &m_pixels[y * m_width];
		}

		/// @brief y 行目の先頭ピクセルへのポインタを返します。
		/// @param y 行番号
		/// @return y 行目の先頭ピクセルへのポインタ
		[[nodiscard]]
		const Color* operator [](int y) const noexcept
		{
			assert((0 <= y) && (y < m_height));
			return &m_pixels[y * m_width];
		}

		/// @brief 指定した位置のピクセルの参照を返します。
		/// @param p ピクセルの位置
		/// @return 指定した位置のピクセルの参照
		[[nodiscard]]
		Color& operator [](const Point& p) noexcept
		{
			assert(inBounds(p.y, p.x));
			return m_pixels[(p.y * m_width) + p.x];
		}

		/// @brief 指定した位置のピクセルの参照を返します。
		/// @param p ピクセルの位置
		/// @return 指定した位置のピクセルの参照
		[[nodiscard]]
		const Color& operator [](const Point& p) const noexcept
		{
			assert(inBounds(p.y, p.x));
			return m_pixels[(p.y * m_width) + p.x];
		}

		/// @brief 画像データの先頭ポインタを返します。
		/// @return 画像データの先頭ポインタ
		[[nodiscard]]
		Color* data() noexcept
		{
			return m_pixels.data();
		}

		/// @brief 画像データの先頭ポインタを返します。
		/// @return 画像データの先頭ポインタ
		[[nodiscard]]
		const Color* data() const noexcept
		{
			return m_pixels.data();
		}

		/// @brief 画像を指定した色で塗りつぶします。
		/// @param fillColor 塗りつぶしの色
		void fill(const Color& fillColor) noexcept
		{
			std::fill(m_pixels.begin(), m_pixels.end(), fillColor);
		}

		/// @brief イテレータの型
		using iterator = std::vector<Color>::iterator;
		
		/// @brief const イテレータの型
		using const_iterator = std::vector<Color>::const_iterator;

		/// @brief 先頭イテレータを返します。
		/// @return 先頭イテレータ
		[[nodiscard]]
		iterator begin() noexcept
		{
			return m_pixels.begin();
		}

		/// @brief 先頭イテレータを返します。
		/// @return 先頭イテレータ
		[[nodiscard]]
		const_iterator begin() const noexcept
		{
			return m_pixels.begin();
		}

		/// @brief 終端イテレータを返します。
		/// @return 終端イテレータ
		[[nodiscard]]
		iterator end() noexcept
		{
			return m_pixels.end();
		}

		/// @brief 終端イテレータを返します。
		/// @return 終端イテレータ
		[[nodiscard]]
		const_iterator end() const noexcept
		{
			return m_pixels.end();
		}

		/// @brief 先頭 const イテレータを返します。
		/// @return 先頭 const イテレータ
		[[nodiscard]]
		const_iterator cbegin() const noexcept
		{
			return m_pixels.cbegin();
		}

		/// @brief 終端 const イテレータを返します。
		/// @return 終端 const イテレータ
		[[nodiscard]]
		const_iterator cend() const noexcept
		{
			return m_pixels.cend();
		}

		/// @brief 指定した行のビューを返します。
		/// @param y 行番号
		/// @return 指定した行のビュー
		[[nodiscard]]
		std::span<Color> row(int y) noexcept
		{
			assert((0 <= y) && (y < m_height));
			return std::span<Color>{ &m_pixels[y * m_width], static_cast<std::size_t>(m_width) };
		}

		/// @brief 指定した行のビューを返します。
		/// @param y 行番号
		///	@return 指定した行のビュー
		[[nodiscard]]
		std::span<const Color> row(int y) const noexcept
		{
			assert((0 <= y) && (y < m_height));
			return std::span<const Color>{ &m_pixels[y * m_width], static_cast<std::size_t>(m_width) };
		}

		bool save(std::string_view fileName) const;

	private:

		/// @brief 画像のピクセルデータ（行優先の一次元配列）
		std::vector<Color> m_pixels;

		/// @brief 画像の幅（ピクセル）
		int m_width = 0;

		/// @brief 画像の高さ（ピクセル）
		int m_height = 0;
	};

	/// @brief BMP 形式で画像を保存します。
	/// @param image 保存する画像
	/// @param fileName 保存先のファイル名
	/// @return 保存に成功した場合 true, それ以外の場合は false
	bool SaveBMP(const Image& image, std::string_view fileName);

	/// @brief BMP 形式の画像を読み込みます。
	/// @param fileName 読み込むファイル名
	/// @return 読み込んだ画像。読み込みに失敗した場合は空の画像を返します。
	[[nodiscard]]
	Image LoadBMP(std::string_view fileName);
}
