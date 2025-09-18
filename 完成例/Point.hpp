#pragma once
#include <cstdint>	// std::int64_t
#include <concepts>	// std::floating_point
#include <cmath>	// std::sqrt
#include <format>	// std::formatter
#include <iostream>	// std::ostream, std::istream

namespace mini
{
	/// @brief 二次元座標を表現するクラス
	struct Point
	{
		/// @brief X 成分
		int x = 0;

		/// @brief Y 成分
		int y = 0;

		/// @brief デフォルトコンストラクタ
		[[nodiscard]]
		Point() = default;

		/// @brief 二次元座標を作成します。
		/// @param _x X 成分
		/// @param _y Y 成分
		[[nodiscard]]
		constexpr Point(int _x, int _y) noexcept
			: x{ _x }
			, y{ _y } {}

		/// @brief 成分がすべて 0 であるかを返します。
		/// @return 成分がすべて 0 である場合 true, それ以外の場合は false
		[[nodiscard]]
		constexpr bool isZero() const noexcept
		{
			return ((x == 0) && (y == 0));
		}

		/// @brief 成分をすべて 0 にします。
		constexpr void clear() noexcept
		{
			x = 0;
			y = 0;
		}

		/// @brief 成分を設定します。
		/// @param _x 新しい X 成分
		/// @param _y 新しい Y 成分
		constexpr void set(int _x, int _y) noexcept
		{
			x = _x;
			y = _y;
		}

		/// @brief 成分を設定します。
		/// @param other 新しい成分
		constexpr void set(const Point& other) noexcept
		{
			x = other.x;
			y = other.y;
		}

		/// @brief 原点からのマンハッタン距離を返します。
		/// @return 原点からのマンハッタン距離
		[[nodiscard]]
		constexpr std::int64_t manhattanDistance() const noexcept
		{
			const std::int64_t x_ = (x < 0) ? -x : x;
			const std::int64_t y_ = (y < 0) ? -y : y;
			return (x_ + y_);
		}

		/// @brief 原点からの距離の 2 乗を返します。
		/// @tparam Type 距離の型
		/// @return 原点からの距離の 2 乗
		template <std::floating_point Type = double>
		[[nodiscard]]
		constexpr Type lengthSq() const noexcept
		{
			const Type x_ = static_cast<Type>(x);
			const Type y_ = static_cast<Type>(y);
			return ((x_ * x_) + (y_ * y_));
		}

		/// @brief 原点からの距離を返します。
		/// @tparam Type 距離の型
		/// @return 原点からの距離
		template <std::floating_point Type = double>
		[[nodiscard]]
		Type length() const noexcept
		{
			return std::sqrt(lengthSq<Type>());
		}

		/// @brief 指定した点からの距離を返します。
		/// @tparam Type 距離の型
		/// @param p もう一方の点の座標
		/// @return もう一方の点からの距離
		template <std::floating_point Type = double>
		[[nodiscard]]
		Type distanceFrom(const Point& other) const noexcept
		{
			return (other - *this).length<Type>();
		}

		[[nodiscard]]
		constexpr Point operator +() const noexcept
		{
			return *this;
		}

		[[nodiscard]]
		constexpr Point operator +(const Point& other) const noexcept
		{
			return{ (x + other.x), (y + other.y) };
		}

		[[nodiscard]]
		constexpr Point operator -() const noexcept
		{
			return{ -x, -y };
		}

		[[nodiscard]]
		constexpr Point operator -(const Point& other) const noexcept
		{
			return{ (x - other.x), (y - other.y) };
		}

		[[nodiscard]]
		constexpr Point operator *(int s) const noexcept
		{
			return{ (x * s), (y * s) };
		}

		[[nodiscard]]
		friend constexpr Point operator *(int s, const Point& other) noexcept
		{
			return{ (s * other.x), (s * other.y) };
		}

		[[nodiscard]]
		constexpr Point operator /(int s) const noexcept
		{
			return{ (x / s), (y / s) };
		}

		[[nodiscard]]
		constexpr Point& operator +=(const Point& other) noexcept
		{
			x += other.x;
			y += other.y;
			return *this;
		}

		[[nodiscard]]
		constexpr Point& operator -=(const Point& other) noexcept
		{
			x -= other.x;
			y -= other.y;
			return *this;
		}

		[[nodiscard]]
		constexpr Point& operator *=(int s) noexcept
		{
			x *= s;
			y *= s;
			return *this;
		}

		[[nodiscard]]
		constexpr Point& operator /=(int s) noexcept
		{
			x /= s;
			y /= s;
			return *this;
		}

		[[nodiscard]]
		friend constexpr bool operator ==(const Point& lhs, const Point& rhs) noexcept = default;

		/// @brief { 0, 0 } を返します。
		/// @return { 0, 0 }
		[[nodiscard]]
		static constexpr Point Zero() noexcept
		{
			return{ 0, 0 };
		}

		/// @brief { 1, 1 } を返します。
		/// @return { 1, 1 }
		[[nodiscard]]
		static constexpr Point One() noexcept
		{
			return{ 1, 1 };
		}

		/// @brief { value, value } を返します。
		/// @param value 成分の値
		/// @return { value, value }
		[[nodiscard]]
		static constexpr Point All(int value = 1) noexcept
		{
			return{ value, value };
		}

		/// @brief 出力ストリームに書き込みます。
		/// @param os 出力ストリーム
		/// @param value 書き込む値
		/// @return 出力ストリーム
		friend std::ostream& operator <<(std::ostream& os, const Point& value)
		{
			return os << '(' << value.x << ", " << value.y << ')';
		}

		/// @brief 入力ストリームから読み込みます。
		/// @param is 入力ストリーム
		/// @param value 読み込んだ値の格納先
		/// @return 入力ストリーム
		friend std::istream& operator >>(std::istream& is, Point& value)
		{
			char _; // 区切り文字用
			return is >> _ >> value.x >> _ >> value.y >> _;
		}
	};
}

template <>
struct std::formatter<mini::Point> : std::formatter<std::string_view>
{
	auto format(const mini::Point& value, auto& ctx) const
	{
		return std::format_to(ctx.out(), "({}, {})", value.x, value.y);
	}
};
