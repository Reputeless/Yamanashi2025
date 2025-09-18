#include <fstream>		// std::ofstream
#include <filesystem>	// std::filesystem::absolute
#include "BinaryFileWriter.hpp"

namespace mini
{
	class BinaryFileWriter::Impl
	{
	public:

		Impl() = default;

		~Impl()
		{
			close();
		}

		[[nodiscard]]
		bool isOpen() const noexcept
		{
			return m_file.is_open();
		}

		bool open(const std::string_view path)
		{
			// すでにオープンされている場合はクローズする
			if (m_file.is_open())
			{
				close();
			}

			// ファイルをバイナリモードでオープンする
			m_file.open(path, std::ios::binary);

			// オープンに失敗した場合は false を返す
			if (!m_file.is_open())
			{
				return false;
			}

			// ファイルの絶対パスを取得して記録する
			m_fullPath = std::filesystem::absolute(path).string();

			return true;
		}

		void close()
		{
			// ファイルをクローズする
			m_file.close();

			// 記録していたファイルの絶対パスをクリアする
			m_fullPath.clear();
		}

		void write(const void* data, const size_t size)
		{
			m_file.write(static_cast<const char*>(data), size);
		}

		[[nodiscard]]
		const std::string& fullPath() const noexcept
		{
			return m_fullPath;
		}

	private:

		/// @brief ファイル出力ストリーム
		std::ofstream m_file;

		/// @brief ファイルの絶対パス
		std::string m_fullPath;
	};

	BinaryFileWriter::BinaryFileWriter()
		: m_pImpl{ std::make_shared<Impl>() } {}

	BinaryFileWriter::BinaryFileWriter(const std::string_view path)
		: BinaryFileWriter{} // 移譲コンストラクタ
	{
		m_pImpl->open(path);
	}

	bool BinaryFileWriter::isOpen() const noexcept
	{
		return m_pImpl->isOpen();
	}

	BinaryFileWriter::operator bool() const noexcept
	{
		return m_pImpl->isOpen();
	}

	bool BinaryFileWriter::open(const std::string_view path)
	{
		return m_pImpl->open(path);
	}

	void BinaryFileWriter::close()
	{
		m_pImpl->close();
	}

	const std::string& BinaryFileWriter::fullPath() const noexcept
	{
		return m_pImpl->fullPath();
	}

	void BinaryFileWriter::write(const void* data, const size_t size)
	{
		m_pImpl->write(data, size);
	}
}
