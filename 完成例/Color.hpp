#pragma once
#include <iostream>		// std::ostream, std::istream
#include <format>		// std::formatter
#include <string_view>	// std::string_view

namespace mini
{
	/// @brief 色を表現するクラス
	struct Color
	{
		/// @brief 赤成分
		double r = 0.0;

		/// @brief 緑成分
		double g = 0.0;

		/// @brief 青成分
		double b = 0.0;

		/// @brief デフォルトコンストラクタ
		[[nodiscard]]
		Color() = default;

		/// @brief 色を作成します。
		/// @param _r 赤成分
		/// @param _g 緑成分
		/// @param _b 青成分
		[[nodiscard]]
		constexpr Color(double _r, double _g, double _b) noexcept
			: r{ _r }
			, g{ _g }
			, b{ _b } {}

		/// @brief グレースケールの色を作成します。
		/// @param rgb 赤成分、緑成分、青成分
		[[nodiscard]]
		explicit constexpr Color(double rgb) noexcept
			: r{ rgb }
			, g{ rgb }
			, b{ rgb } {}

		[[nodiscard]]
		constexpr Color operator +() const noexcept
		{
			return *this;
		}

		[[nodiscard]]
		constexpr Color operator -() const noexcept
		{
			return{ -r, -g, -b };
		}

		[[nodiscard]]
		constexpr Color operator +(const Color& other) const noexcept
		{
			return{ (r + other.r), (g + other.g), (b + other.b) };
		}

		[[nodiscard]]
		constexpr Color operator -(const Color& other) const noexcept
		{
			return{ (r - other.r), (g - other.g), (b - other.b) };
		}

		[[nodiscard]]
		constexpr Color operator *(double s) const noexcept
		{
			return{ (r * s), (g * s), (b * s) };
		}

		[[nodiscard]]
		friend constexpr Color operator *(double s, const Color& other) noexcept
		{
			return{ (s * other.r), (s * other.g), (s * other.b) };
		}

		[[nodiscard]]
		constexpr Color operator /(double s) const noexcept
		{
			return{ (r / s), (g / s), (b / s) };
		}

		[[nodiscard]]
		constexpr Color& operator +=(const Color& other) noexcept
		{
			r += other.r;
			g += other.g;
			b += other.b;
			return *this;
		}

		[[nodiscard]]
		constexpr Color& operator -=(const Color& other) noexcept
		{
			r -= other.r;
			g -= other.g;
			b -= other.b;
			return *this;
		}

		[[nodiscard]]
		constexpr Color& operator *=(double s) noexcept
		{
			r *= s;
			g *= s;
			b *= s;
			return *this;
		}

		[[nodiscard]]
		constexpr Color& operator /=(double s) noexcept
		{
			r /= s;
			g /= s;
			b /= s;
			return *this;
		}

		/// @brief グレースケール値を返します。
		/// @return グレースケール値
		[[nodiscard]]
		constexpr double grayscale() const noexcept
		{
			return (0.299 * r) + (0.587 * g) + (0.114 * b);
		}

		/// @brief 出力ストリームに書き込みます。
		/// @param os 出力ストリーム
		/// @param value 書き込む値
		/// @return 出力ストリーム
		friend std::ostream& operator <<(std::ostream& os, const Color& value)
		{
			return os << '(' << value.r << ", " << value.g << ", " << value.b << ')';
		}

		/// @brief 入力ストリームから読み込みます。
		/// @param is 入力ストリーム
		/// @param value 読み込んだ値の格納先
		/// @return 入力ストリーム
		friend std::istream& operator >>(std::istream& is, Color& value)
		{
			char _; // 区切り文字用
			return is >> _ >> value.r >> _ >> value.g >> _ >> value.b >> _;
		}
	};
}

template <>
struct std::formatter<mini::Color> : std::formatter<std::string_view>
{
	auto format(const mini::Color& value, auto& ctx) const
	{
		return std::format_to(ctx.out(), "({}, {}, {})", value.r, value.g, value.b);
	}
};
