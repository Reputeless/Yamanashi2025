#include <fstream> // std::ifstream
#include <filesystem> // std::filesystem::absolute
#include "BinaryFileReader.hpp"

namespace mini
{
	// 無名名前空間（この中の関数を、別の翻訳単位からは見えなくする）
	namespace
	{
		/// @brief ファイルのサイズ（バイト）を取得します。
		/// @param file ファイル
		/// @return ファイルのサイズ（バイト）
		[[nodiscard]]
		std::int64_t GetFileSize(std::ifstream& file)
		{
			const std::int64_t currentPos = file.tellg();

			file.seekg(0, std::ios::end);

			const std::int64_t size = file.tellg();

			file.seekg(currentPos);

			return size;
		}
	}

	class BinaryFileReader::Impl
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

			// ファイルの絶対パスとサイズを取得して記録する
			m_fullPath = std::filesystem::absolute(path).string();
			m_size = GetFileSize(m_file);

			return true;
		}

		void close()
		{
			// ファイルをクローズする
			m_file.close();

			// 記録していたファイルの絶対パスとサイズをクリアする
			m_fullPath.clear();
			m_size = 0;
		}

		[[nodiscard]]
		std::int64_t size() const noexcept
		{
			return m_size;
		}

		[[nodiscard]]
		std::int64_t read(void* data, const size_t size)
		{
			m_file.read(static_cast<char*>(data), size);

			// 直近の read で読み込んだバイト数を返す
			return m_file.gcount();
		}

		[[nodiscard]]
		const std::string& fullPath() const noexcept
		{
			return m_fullPath;
		}

	private:

		/// @brief ファイル入力ストリーム
		std::ifstream m_file;

		/// @brief ファイルのサイズ（バイト）
		std::int64_t m_size = 0;

		/// @brief ファイルの絶対パス
		std::string m_fullPath;
	};

	BinaryFileReader::BinaryFileReader()
		: m_pImpl{ std::make_shared<Impl>() } {}

	BinaryFileReader::BinaryFileReader(const std::string_view path)
		: BinaryFileReader{} // 移譲コンストラクタ
	{
		m_pImpl->open(path);
	}

	bool BinaryFileReader::isOpen() const noexcept
	{
		return m_pImpl->isOpen();
	}

	BinaryFileReader::operator bool() const noexcept
	{
		return m_pImpl->isOpen();
	}

	bool BinaryFileReader::open(const std::string_view path)
	{
		return m_pImpl->open(path);
	}

	void BinaryFileReader::close()
	{
		m_pImpl->close();
	}

	std::int64_t BinaryFileReader::size() const noexcept
	{
		return m_pImpl->size();
	}

	std::int64_t BinaryFileReader::read(void* data, const size_t size)
	{
		return m_pImpl->read(data, size);
	}

	const std::string& BinaryFileReader::fullPath() const noexcept
	{
		return m_pImpl->fullPath();
	}
}
