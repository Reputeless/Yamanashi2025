#include <print>
#include "Point.hpp"
#include "BinaryFileWriter.hpp"
#include "BinaryFileReader.hpp"
#include "Color.hpp"
#include "Image.hpp"

using namespace mini;

int main()
{
	std::println("こんにちは、セキュリティキャンプ！\n");
	std::println("C++{}", (10 + 13));

	std::println("---- Point.hpp ----");
	{
		std::println("{}", (Point{ 100, 200 } + Point{ 1, 2 }));
		std::println("{}", (2 * Point{ 100, 200 }));
		std::println("{}", (Point{ 1, 2 } == Point{ 1, 2 }));
	}

	std::println("---- BinaryFileWriter.hpp ----");
	{
		BinaryFileWriter writer{ "test.bin" };
		std::println("writer.isOpen(): {}", writer.isOpen());
		std::println("writer.fullPath(): {}", writer.fullPath());

		// trivially copyable な型は簡潔に書き込みできる
		const int a = 123;
		const double b = 1.25;
		const Point p{ 100, 200 };
		writer.write(a);
		writer.write(b);
		writer.write(p);

		// そうでない型は適切に
		const std::string str = "Hello, Yamanashi!";
		const std::uint64_t length = str.size();
		writer.write(length);
		writer.write(str.data(), str.size());
	}

	std::println("---- BinaryFileReader.hpp ----");
	{
		BinaryFileReader reader{ "test.bin" };
		std::println("reader.isOpen(): {}", reader.isOpen());
		std::println("reader.fullPath(): {}", reader.fullPath());
		std::println("reader.size(): {}", reader.size());

		// trivially copyable な型は簡潔に読み込みできる
		int a;
		double b;
		Point p;
		reader.read(a);
		reader.read(b);
		reader.read(p);
		std::println("a: {}", a);
		std::println("b: {}", b);
		std::println("p: {}", p);

		// そうでない型は適切に
		std::uint64_t length;
		reader.read(length);
		std::println("length: {}", length);
		std::string str;
		str.resize(length);
		reader.read(str.data(), str.size());
		std::println("str: {}", str);
	}

	std::println("---- Color.hpp ----");
	{
		std::println("{}", Color{ 1.0 });
		std::println("{}", Color{ 0.0, 1.0, 0.0 });
		std::println("{}", (Color{ 0.1, 0.2, 0.3 } + Color{ 0.1, 0.2, 0.3 }));
		std::println("{}", Color{ 0.6, 0.8, 1.0 }.grayscale());
	}

	std::println("---- Image.hpp ----");
	{
		{
			Image image(400, 300, Color{ 0.8, 0.9, 1.0 });

			for (int y = 40; y < 60; ++y)
			{
				for (int x = 0; x < image.width(); ++x)
				{
					image[y][x] = Color{ 0.0 };
				}
			}

			if (image.save("test1.bmp"))
			{
				std::println("test1.bmp を保存しました。");
			}
		}

		{
			Image image{ "test1.bmp" };

			for (int y = 0; y < image.height(); ++y)
			{
				for (int x = 40; x < 60; ++x)
				{
					image[y][x] = Color{ 1.0 };
				}
			}

			if (image.save("test2.bmp"))
			{
				std::println("test2.bmp を保存しました。");
			}
		}

		// input.bmp を output.bmp として保存するだけ
		//Image{ "input.bmp" }.save("output.bmp");
	}
}
