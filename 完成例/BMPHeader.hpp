#pragma once
#include <cstdint> // std::uint16_t, std::uint32_t, std::int32_t

namespace mini
{
#pragma pack(push, 2)  // 2 バイトアライメントに変更

	/// @brief BMP ファイルのヘッダー情報を表すクラス
	struct BMPHeader
	{
		std::uint16_t bfType = 0x4D42;	// B と M の 2 バイト
		std::uint32_t bfSize = 0;		// ファイル全体のサイズ（バイト）
		std::uint16_t bfReserved1 = 0;
		std::uint16_t bfReserved2 = 0;
		std::uint32_t bfOffBits = sizeof(BMPHeader); // ヘッダーのサイズ（バイト）
		std::uint32_t biSize = 40;		// 40（この値は固定）
		std::int32_t  biWidth = 0;		// 画像の幅（ピクセル）
		std::int32_t  biHeight = 0;		// 画像の高さ（ピクセル）。正の場合は下の行から、負の場合は上の行から格納
		std::uint16_t biPlanes = 1;		// 1（この値は固定）
		std::uint16_t biBitCount = 24;	// 24 ビットカラーの場合は 24
		std::uint32_t biCompression = 0;
		std::uint32_t biSizeImage = 0;	// 画像データのサイズ（バイト）
		std::int32_t  biXPelsPerMeter = 0;
		std::int32_t  biYPelsPerMeter = 0;
		std::uint32_t biClrUsed = 0;
		std::uint32_t biClrImportant = 0;

		/// @brief 指定した幅と高さに基づいて BMPHeader を作成します。
		/// @param width 画像の幅（ピクセル）
		/// @param height 画像の高さ（ピクセル）
		/// @return 作成した BMPHeader
		[[nodiscard]]
		static constexpr BMPHeader Make(int width, int height) noexcept
		{
			BMPHeader header;
			header.biWidth = width;
			header.biHeight = height;
			const int rowSize = ((width * 3 + 3) / 4) * 4; // 4 バイト境界に合わせる
			header.bfSize = sizeof(BMPHeader) + (rowSize * height);
			header.biSizeImage = (rowSize * height);
			return header;
		}
	};

#pragma pack(pop) // アライメント設定を元に戻す

	// BMPHeader のサイズが 54 バイトであることをコンパイル時にチェック
	static_assert(sizeof(BMPHeader) == 54, "BMPHeader size must be 54 bytes");
}
