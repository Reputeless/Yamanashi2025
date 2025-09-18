#include <vector>				// std::vector
#include <string_view>			// std::string_view
#include <algorithm>			// std::clamp
#include <cmath>				// std::abs
#include <cstdint>				// std::uint8_t
#include "Image.hpp"			// mini::Image
#include "BMPHeader.hpp"		// mini::BMPHeader
#include "BinaryFileWriter.hpp"	// mini::BinaryFileWriter
#include "BinaryFileReader.hpp" // mini::BinaryFileReader

namespace mini
{
	Image::Image(std::string_view fileName)
	{
		*this = LoadBMP(fileName);
	}

	bool Image::save(std::string_view fileName) const
	{
		return SaveBMP(*this, fileName);
	}

	bool SaveBMP(const Image& image, std::string_view fileName)
	{
		const int width = image.width();
		const int height = image.height();
		const int rowSize = ((width * 3 + 3) / 4) * 4; // 4 バイト境界に合わせる
		const BMPHeader header = BMPHeader::Make(width, height);

		BinaryFileWriter writer{ fileName };

		// ファイルがオープンされていない場合は失敗
		if (!writer)
		{
			return false;
		}

		// ヘッダーを書き込む
		writer.write(header);

		// 1 行分のデータを格納するバッファ
		std::vector<std::uint8_t> rowData(rowSize, 0);

		for (int y = 0; y < height; ++y)
		{
			// BMP は下の行から格納するので、y は height - 1 - y でアクセスする
			for (int x = 0; x < width; ++x)
			{
				const Color color = image[height - 1 - y][x];

				// 各色成分を、保存のため 8 ビット整数（0 ～ 255）に変換する
				const std::uint8_t r = static_cast<std::uint8_t>(std::clamp((color.r * 255.0 + 0.5), 0.0, 255.0));
				const std::uint8_t g = static_cast<std::uint8_t>(std::clamp((color.g * 255.0 + 0.5), 0.0, 255.0));
				const std::uint8_t b = static_cast<std::uint8_t>(std::clamp((color.b * 255.0 + 0.5), 0.0, 255.0));

				rowData[x * 3 + 0] = b; // 青
				rowData[x * 3 + 1] = g; // 緑
				rowData[x * 3 + 2] = r; // 赤
			}

			// 1 行分のデータを書き込む
			writer.write(rowData.data(), rowSize);
		}

		return true;
	}

	Image LoadBMP(std::string_view fileName)
	{
		BinaryFileReader reader{ fileName };

		// ファイルがオープンされていない場合は失敗
		if (!reader)
		{
			return{};
		}

		BMPHeader header;

		// ヘッダーサイズ分のデータを読み込めない場合は失敗
		if (reader.read(header) != sizeof(BMPHeader))
		{
			return{};
		}

		// BMP 形式でない場合、または 24 ビットカラーでない場合は失敗（これ以外にもチェックを強化できる）
		if ((header.bfType != 0x4D42) || (header.biBitCount != 24))
		{
			return{};
		}

		const int width = header.biWidth;
		const int height = std::abs(header.biHeight); // 負の場合は上の行から格納されている
		const int rowSize = ((width * 3 + 3) / 4) * 4; // 4 バイト境界に合わせる

		Image image{ width, height };
		std::vector<std::uint8_t> rowData(rowSize);

		for (int y = 0; y < height; ++y)
		{
			// 1 行分のデータを読み込む
			if (reader.read(rowData.data(), rowSize) != rowSize)
			{
				return{};
			}

			for (int x = 0; x < width; ++x)
			{
				const std::uint8_t b = rowData[x * 3 + 0]; // 青
				const std::uint8_t g = rowData[x * 3 + 1]; // 緑
				const std::uint8_t r = rowData[x * 3 + 2]; // 赤

				// 各色成分を、0.0 ～ 1.0 の範囲の実数に変換する
				const Color color{ (r / 255.0), (g / 255.0), (b / 255.0) };

				if (0 < header.biHeight)
				{
					// 正の場合は下の行から格納されている
					image[height - 1 - y][x] = color;
				}
				else
				{
					// 負の場合は上の行から格納されている
					image[y][x] = color;
				}
			}
		}

		return image;
	}
}
