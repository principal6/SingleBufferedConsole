#pragma once

#include <Windows.h>
#include <iostream>
#include <string>
#include <conio.h>

enum class ECommandLinePosition
{
	Bottom,
	Top,
};

enum class EArrowKeys
{
	None,
	Up,
	Left,
	Right,
	Down
};

enum class EBackgroundColor
{
	Black = 0,
	DarkGray = BACKGROUND_INTENSITY,
	LightGray = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE,
	White = BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE,

	Red = BACKGROUND_RED,
	Green = BACKGROUND_GREEN,
	Blue = BACKGROUND_BLUE,
	Cyan = BACKGROUND_GREEN | BACKGROUND_BLUE,
	Magenta = BACKGROUND_RED | BACKGROUND_BLUE,
	Yellow = BACKGROUND_RED | BACKGROUND_GREEN,

	LightRed = BACKGROUND_INTENSITY | BACKGROUND_RED,
	LightGreen = BACKGROUND_INTENSITY | BACKGROUND_GREEN,
	LightBlue = BACKGROUND_INTENSITY | BACKGROUND_BLUE,
	LightCyan = BACKGROUND_INTENSITY | BACKGROUND_GREEN | BACKGROUND_BLUE,
	LightMagenta = BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_BLUE,
	LightYellow = BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN,
};

enum class EForegroundColor
{
	Black = 0,
	DarkGray = FOREGROUND_INTENSITY,
	LightGray = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
	White = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,

	Red = FOREGROUND_RED,
	Green = FOREGROUND_GREEN,
	Blue = FOREGROUND_BLUE,
	Cyan = FOREGROUND_GREEN | FOREGROUND_BLUE,
	Magenta = FOREGROUND_RED | FOREGROUND_BLUE,
	Yellow = FOREGROUND_RED | FOREGROUND_GREEN,

	LightRed = FOREGROUND_INTENSITY | FOREGROUND_RED,
	LightGreen = FOREGROUND_INTENSITY | FOREGROUND_GREEN,
	LightBlue = FOREGROUND_INTENSITY | FOREGROUND_BLUE,
	LightCyan = FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE,
	LightMagenta = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE,
	LightYellow = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN,
};

class SingleBufferedConsole
{
public:
	SingleBufferedConsole(short width, short height, const char* title, ECommandLinePosition eCommandLinePosition) :
		_width{ width }, _height{ height }, _eCommandLinePosition{ eCommandLinePosition } 
	{
		startUp(title); 
	}
	virtual ~SingleBufferedConsole() 
	{
		cleanUp(); 
	}

public:
	virtual void clear() noexcept
	{
		CHAR_INFO emptyChar{};
		emptyChar.Attributes = (WORD)((WORD)_eClearBackground | (WORD)_eDefaultForeground);
		for (int i = 0; i < _bufferSize; ++i)
		{
			_buffer[i] = emptyChar;
		}
	}

public:
	virtual void printChar(short x, short y, char ch, WORD attribute) noexcept
	{
		int index{ y * _width + x };
		_buffer[index].Attributes = attribute;
		_buffer[index].Char.AsciiChar = ch;
	}

	virtual void printChar(short x, short y, char ch, EBackgroundColor eBackground, EForegroundColor eForeground) noexcept
	{
		printChar(x, y, ch, getNewAttribute(eBackground, eForeground));
	}

	virtual void printChar(short x, short y, char ch, EBackgroundColor eBackground) noexcept
	{
		printChar(x, y, ch, getPatchedAttribute(x, y, eBackground));
	}

	virtual void printChar(short x, short y, char ch, EForegroundColor eForeground) noexcept
	{
		printChar(x, y, ch, getPatchedAttribute(x, y, eForeground));
	}

	virtual void printChar(short x, short y, char ch) noexcept
	{
		printChar(x, y, ch, getCurrentAttribute(x, y));
	}

public:
	virtual void printHorzString(short x, short y, const char* str, WORD attribute, int strLength = -1) noexcept
	{
		if (strLength == -1) strLength = (int)strlen(str);

		int offset{ y * _width + x };
		for (int i = 0; i < strLength; ++i)
		{
			_buffer[offset + i].Attributes = attribute;
			_buffer[offset + i].Char.AsciiChar = str[i];
		}
	}

	virtual void printHorzString(short x, short y, const char* str, EBackgroundColor eBackground, EForegroundColor eForeground, int strLength = -1) noexcept
	{
		printHorzString(x, y, str, getNewAttribute(eBackground, eForeground), strLength);
	}

	virtual void printHorzString(short x, short y, const char* str, EBackgroundColor eBackground, int strLength = -1) noexcept
	{
		printHorzString(x, y, str, getPatchedAttribute(x, y, eBackground), strLength);
	}

	virtual void printHorzString(short x, short y, const char* str, EForegroundColor eForeground, int strLength = -1) noexcept
	{
		printHorzString(x, y, str, getPatchedAttribute(x, y, eForeground), strLength);
	}

	virtual void printHorzString(short x, short y, const char* str, int strLength = -1) noexcept
	{
		printHorzString(x, y, str, getCurrentAttribute(x, y), strLength);
	}

	virtual void printHorzString(short x, short y, const std::string& str) noexcept
	{
		printHorzString(x, y, str.c_str(), (int)str.size());
	}

	virtual void printHorzString(short x, short y, short shortValue) noexcept
	{
		std::string str{ std::to_string((int)shortValue) };
		printHorzString(x, y, str.c_str(), (int)str.size());
	}

public:
	virtual void printVertString(short x, short y, const char* str, WORD attribute, int strLength = -1) noexcept
	{
		if (strLength == -1) strLength = (int)strlen(str);

		int offset{ y * _width + x };
		for (int i = 0; i < strLength; ++i)
		{
			_buffer[offset + (i * _width)].Attributes = attribute;
			_buffer[offset + (i * _width)].Char.AsciiChar = str[i];
		}
	}

	virtual void printVertString(short x, short y, const char* str, EBackgroundColor eBackground, EForegroundColor eForeground, int strLength = -1) noexcept
	{
		printVertString(x, y, str, getNewAttribute(eBackground, eForeground), strLength);
	}

	virtual void printVertString(short x, short y, const char* str, EBackgroundColor eBackground, int strLength = -1) noexcept
	{
		printVertString(x, y, str, getPatchedAttribute(x, y, eBackground), strLength);
	}

	virtual void printVertString(short x, short y, const char* str, EForegroundColor eForeground, int strLength = -1) noexcept
	{
		printVertString(x, y, str, getPatchedAttribute(x, y, eForeground), strLength);
	}

	virtual void printVertString(short x, short y, const char* str, int strLength = -1) noexcept
	{
		printVertString(x, y, str, getCurrentAttribute(x, y), strLength);
	}

public:
	virtual void fillHorzChar(short x, short y, char ch, int count, WORD attribute) noexcept
	{
		int offset{ y * _width + x };
		for (int i = 0; i < count; ++i)
		{
			_buffer[offset + i].Attributes = attribute;
			_buffer[offset + i].Char.AsciiChar = ch;
		}
	}

	virtual void fillHorzChar(short x, short y, char ch, int count, EBackgroundColor eBackground, EForegroundColor eForeground) noexcept
	{
		fillHorzChar(x, y, ch, count, getNewAttribute(eBackground, eForeground));
	}

	virtual void fillHorzChar(short x, short y, char ch, int count, EBackgroundColor eBackground) noexcept
	{
		fillHorzChar(x, y, ch, count, getPatchedAttribute(x, y, eBackground));
	}

	virtual void fillHorzChar(short x, short y, char ch, int count, EForegroundColor eForeground) noexcept
	{
		fillHorzChar(x, y, ch, count, getPatchedAttribute(x, y, eForeground));
	}

	virtual void fillHorzChar(short x, short y, char ch, int count) noexcept
	{
		fillHorzChar(x, y, ch, count, getCurrentAttribute(x, y));
	}

public:
	virtual void fillVertChar(short x, short y, char ch, int count, WORD attribute) noexcept
	{
		int offset{ y * _width + x };
		for (int i = 0; i < count; ++i)
		{
			_buffer[offset + (i * _width)].Attributes = attribute;
			_buffer[offset + (i * _width)].Char.AsciiChar = ch;
		}
	}

	virtual void fillVertChar(short x, short y, char ch, int count, EBackgroundColor eBackground, EForegroundColor eForeground) noexcept
	{
		fillVertChar(x, y, ch, count, getNewAttribute(eBackground, eForeground));
	}

	virtual void fillVertChar(short x, short y, char ch, int count, EBackgroundColor eBackground) noexcept
	{
		fillVertChar(x, y, ch, count, getPatchedAttribute(x, y, eBackground));
	}

	virtual void fillVertChar(short x, short y, char ch, int count, EForegroundColor eForeground) noexcept
	{
		fillVertChar(x, y, ch, count, getPatchedAttribute(x, y, eForeground));
	}

	virtual void fillVertChar(short x, short y, char ch, int count) noexcept
	{
		fillVertChar(x, y, ch, count, getCurrentAttribute(x, y));
	}

public:
	virtual void printBox(short x, short y, short Width, short Height, char ch, EBackgroundColor eBackground, EForegroundColor eForeground) noexcept
	{
		fillHorzChar(x, y, ch, Width, eBackground, eForeground);
		fillHorzChar(x, y + Height - 1, ch, Width, eBackground, eForeground);
		fillVertChar(x, y, ch, Height, eBackground, eForeground);
		fillVertChar(x + Width - 1, y, ch, Height, eBackground, eForeground);
	}

	virtual void fillBox(short x, short y, short Width, short Height, char ch, EBackgroundColor eBackground, EForegroundColor eForeground) noexcept
	{
		for (short i = 0; i < Height; ++i)
		{
			fillHorzChar(x, y + i, ch, Width, eBackground, eForeground);
		}
	}

protected:
	virtual WORD getCurrentAttribute(short x, short y) const noexcept
	{
		int index{ y * _width + x };
		return _buffer[index].Attributes;
	}

	virtual WORD getNewAttribute(EBackgroundColor eBackground, EForegroundColor eForeground) const noexcept
	{
		return (WORD)((WORD)eBackground | (WORD)eForeground);
	}

	virtual WORD getPatchedAttribute(short X, short Y, EBackgroundColor eBackground) const noexcept
	{
		return (WORD)((getCurrentAttribute(X, Y) & 0x000F) + (WORD)eBackground);
	}

	virtual WORD getPatchedAttribute(short X, short Y, EForegroundColor eForeground) const noexcept
	{
		return (WORD)((getCurrentAttribute(X, Y) & 0x00F0) + (WORD)eForeground);
	}

public:
	virtual void render() const noexcept
	{
		SMALL_RECT rect{};
		rect.Top = (_eCommandLinePosition == ECommandLinePosition::Bottom) ? 0 : 1;
		rect.Right = _width;
		rect.Bottom = _height + rect.Top - 1;
		WriteConsoleOutputA(
			_output,
			_buffer,
			COORD{ rect.Right, rect.Bottom - rect.Top },
			COORD{ 0, 0 },
			&rect);

		COORD coord{ 0, (_eCommandLinePosition == ECommandLinePosition::Bottom) ? _height - 1 : 0 };
		DWORD writtenCount{};
		if (isReadingCommand())
		{
			WriteConsoleOutputCharacterA(_output, "> ", 2, coord, &writtenCount);

			short commandBufferSize{ (short)strlen(_commandBuffer) };
			coord.X += 2;
			WriteConsoleOutputCharacterA(_output, _commandBuffer, commandBufferSize, coord, &writtenCount);

			wchar_t wsEnd[2]{ 0x25c4 };
			coord.X += commandBufferSize;
			WriteConsoleOutputCharacterW(_output, wsEnd, 1, coord, &writtenCount);

			coord.X += 1;
			FillConsoleOutputCharacterA(_output, ' ', _width - coord.X, coord, &writtenCount);
		}
		else
		{
			FillConsoleOutputCharacterA(_output, ' ', _width, coord, &writtenCount);
		}
	}

public:
	virtual void setClearBackground(EBackgroundColor eBackground) noexcept
	{
		_eClearBackground = eBackground;
	}

	virtual void setDefaultForeground(EForegroundColor eForeground) noexcept
	{
		_eDefaultForeground = eForeground;
	}

public:
	virtual bool hitKey() noexcept
	{
		if (GetAsyncKeyState(VK_LMENU) < 0 && GetAsyncKeyState(VK_RETURN) < 0)
		{
			reset();
			return false;
		}

		if (_kbhit())
		{
			_hitKey = _getch();

			if (_hitKey == 224)
			{
				_hitKey = 0;
				int key{ _getch() }; // arrow keys (up 72, left 75, right 77, down 80)
				if (key == 72) _eHitArrowKey = EArrowKeys::Up;
				else if (key == 75) _eHitArrowKey = EArrowKeys::Left;
				else if (key == 77) _eHitArrowKey = EArrowKeys::Right;
				else if (key == 80) _eHitArrowKey = EArrowKeys::Down;
				else _eHitArrowKey = EArrowKeys::None;
			}
			else
			{
				_eHitArrowKey = EArrowKeys::None;
			}

			return true;
		}
		_hitKey = 0;
		return false;
	}

	virtual bool isKeyHit(int keyCode) const noexcept
	{
		return (_hitKey == keyCode);
	}

	virtual bool isKeyHit(EArrowKeys eArrowKey) const noexcept
	{
		return (_eHitArrowKey == eArrowKey);
	}

public:
	virtual bool isReadingCommand() const noexcept { return _bReadingCommand; }
	virtual bool readCommand() noexcept
	{
		// Initialize variables
		_bReadingCommand = true;
		_commandReadBytes = 0;
		int currentLogIndex{ _commandLogIndex };
		while (true)
		{
			if (_kbhit())
			{
				bool bShouldRead{ true };
				int key{ _getch() };
				if (key == 224)
				{
					int arrowKey{ _getch() };
					if (arrowKey == 72) // Up
					{
						memset(_commandBuffer, 0, kCommandBufferSize);

						--currentLogIndex;
						if (currentLogIndex < 0) currentLogIndex = kCommandLogSize - 1;

						memcpy(_commandBuffer, _commandLog[currentLogIndex], strlen(_commandLog[currentLogIndex]));
						_commandReadBytes = (short)strlen(_commandBuffer);
						bShouldRead = false;
					}
					else if (arrowKey == 80) // Down
					{
						memset(_commandBuffer, 0, kCommandBufferSize);

						++currentLogIndex;
						if (currentLogIndex >= kCommandLogSize) currentLogIndex = 0;

						memcpy(_commandBuffer, _commandLog[currentLogIndex], strlen(_commandLog[currentLogIndex]));
						_commandReadBytes = (short)strlen(_commandBuffer);
						bShouldRead = false;
					}
					else
					{
						continue;
					}
				}
				if (key == VK_ESCAPE)
				{
					_commandReadBytes = 0;
					break;
				}
				if (key == VK_RETURN)
				{
					break;
				}
				if (key == VK_BACK)
				{
					if (_commandReadBytes)
					{
						if (_commandBuffer[_commandReadBytes - 1] < 0)
						{
							// Non-ASCII

							_commandBuffer[_commandReadBytes - 2] = 0;
							_commandBuffer[_commandReadBytes - 1] = 0;

							_commandReadBytes -= 2;
						}
						else
						{
							// ASCII

							--_commandReadBytes;
							_commandBuffer[_commandReadBytes] = 0;
						}
					}
					bShouldRead = false;
				}

				if (bShouldRead)
				{
					if (_commandReadBytes >= kCommandBufferSize - 1 || _commandReadBytes >= _width - 3) continue;

					_commandBuffer[_commandReadBytes] = key;
					++_commandReadBytes;
				}
			}
		}

		_bReadingCommand = false;
		if (_commandReadBytes)
		{
			memcpy(_commandLog[_commandLogIndex], _commandBuffer, _commandReadBytes);
			++_commandLogIndex;
			if (_commandLogIndex >= kCommandLogSize) _commandLogIndex = 0;
		}
		memset(_commandBuffer, 0, kCommandBufferSize);
		return _commandReadBytes;
	}

	virtual const char* getLastCommand() const noexcept { return _commandLog[(_commandLogIndex == 0) ? kCommandLogSize : _commandLogIndex - 1]; }

	virtual bool isLastCommand(const char* cmp) const noexcept
	{
		if (!cmp) return false;
		int length{ (int)strlen(cmp) };
		if (strncmp(getLastCommand(), cmp, length) == 0)
		{
			return true;
		}
		return false;
	}

	// prints log bottom-up
	// x, y, width, height are outer-box size
	virtual void printCommandLog(short x, short y, short width, short height) noexcept
	{
		x += 1;
		y += 1;
		width -= 2;
		height -= 2;

		short height_{ min(min(kCommandLogSize, _height), height) };
		for (short i = 0; i < height_; ++i)
		{
			short currentLogIndex{ _commandLogIndex - 1 - i };
			if (currentLogIndex < 0) currentLogIndex += kCommandLogSize;
			short y_{ max(y + height_ - i - 1, y) };

			printHorzString(x, y_, _commandLog[currentLogIndex], _eClearBackground, EForegroundColor::LightYellow,
				min(width, static_cast<short>(strlen(_commandLog[currentLogIndex]))));
		}
	}

public:
	virtual void terminate() noexcept { _bRunning = false; }
	virtual bool isTerminated() const noexcept { return !_bRunning; }

protected:
	virtual void startUp(const char* title)
	{
		_window = GetConsoleWindow();
		_output = GetStdHandle(STD_OUTPUT_HANDLE);

		cleanUp();

		_bufferSize = _width * (_height - 1);
		_buffer = new CHAR_INFO[_bufferSize]{};

		SetConsoleTitleA(title);

		reset();

		_bRunning = true;
	}

	virtual void reset()
	{
		CONSOLE_SCREEN_BUFFER_INFOEX bufferInfo{};
		bufferInfo.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
		GetConsoleScreenBufferInfoEx(_output, &bufferInfo);

		WORD attribute{ (WORD)((WORD)_eClearBackground | (WORD)_eDefaultForeground) };
		int bufferSize{ bufferInfo.dwSize.X * bufferInfo.dwSize.Y };
		CHAR_INFO* whiteSpaceBuffer = new CHAR_INFO[bufferSize]{};
		for (int i = 0; i < bufferSize; ++i)
		{
			whiteSpaceBuffer[i].Char.AsciiChar = ' ';
			whiteSpaceBuffer[i].Attributes = attribute;
		}
		WriteConsoleOutputA(_output, whiteSpaceBuffer, bufferInfo.dwSize, COORD{ 0, 0 }, &bufferInfo.srWindow);
		delete[] whiteSpaceBuffer;
		whiteSpaceBuffer = nullptr;
		
		bufferInfo.dwMaximumWindowSize.X = bufferInfo.srWindow.Right = bufferInfo.dwSize.X = _width;
		bufferInfo.dwMaximumWindowSize.Y = bufferInfo.srWindow.Bottom = bufferInfo.dwSize.Y = _height;
		SetConsoleScreenBufferInfoEx(_output, &bufferInfo);

		CONSOLE_CURSOR_INFO cursorInfo{ sizeof(CONSOLE_CURSOR_INFO), false };
		SetConsoleCursorInfo(_output, &cursorInfo);

		CONSOLE_HISTORY_INFO historyInfo{};
		historyInfo.cbSize = sizeof(CONSOLE_HISTORY_INFO);
		historyInfo.dwFlags = 0;
		historyInfo.HistoryBufferSize = 0;
		historyInfo.NumberOfHistoryBuffers = 0;
		SetConsoleHistoryInfo(&historyInfo);
	}

	virtual void cleanUp()
	{
		if (_buffer)
		{
			delete[] _buffer;
			_buffer = nullptr;
		}
	}

protected:
	static constexpr short	kCommandBufferSize{ 256 };
	static constexpr short	kCommandLogSize{ 30 };

protected:
	bool					_bRunning{};
	short					_width{};
	short					_height{};
	HWND					_window{};
	HANDLE					_output{};

protected:
	CHAR_INFO*				_buffer{};
	int						_bufferSize{};

protected:
	EBackgroundColor		_eClearBackground{};
	EForegroundColor		_eDefaultForeground{ EForegroundColor::White };

protected:
	mutable int				_hitKey{};
	mutable EArrowKeys		_eHitArrowKey{};

protected:
	ECommandLinePosition	_eCommandLinePosition{ ECommandLinePosition::Bottom };
	bool					_bReadingCommand{};
	char					_commandBuffer[kCommandBufferSize]{};
	short					_commandReadBytes{};
	char					_commandLog[kCommandLogSize][kCommandBufferSize]{};
	short					_commandLogIndex{};
};
