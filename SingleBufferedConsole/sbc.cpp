#include "SingleBufferedConsole.h"
#include <string>
#include <thread>
#include <atomic>

int main()
{
	CSingleBufferedConsole Console{ 130, 30, "Console", ECommandLinePosition::Bottom };
	Console.SetClearBackground(EBackgroundColor::Black);
	Console.SetDefaultForeground(EForegroundColor::White);

	std::atomic<short> X{};
	std::atomic<short> Y{};
	std::thread ThreadInput
	{
		[&]()
		{
			while (true)
			{
				if (Console.IsTerminated()) break;

				if (Console.HitKey())
				{
					if (Console.IsHitKey(EArrowKeys::Right)) ++X;
					if (Console.IsHitKey(EArrowKeys::Left)) --X;
					if (Console.IsHitKey(EArrowKeys::Down)) ++Y;
					if (Console.IsHitKey(EArrowKeys::Up)) --Y;
					if (Console.IsHitKey(VK_RETURN))
					{
						if (Console.ReadCommand())
						{
							if (Console.IsLastCommand("/quit"))
							{
								Console.Terminate();
							}
						}
					}
				}
			}
		}
	};

	while (true)
	{
		if (Console.IsTerminated()) break;

		Console.Clear();

		Console.FillBox(5, 5, 7, 10, '~', EBackgroundColor::Cyan, EForegroundColor::White);

		Console.PrintBox(0, 0, 70, 29, ' ', EBackgroundColor::DarkGray, EForegroundColor::Black);

		Console.PrintBox(70, 0, 40, 29, ' ', EBackgroundColor::DarkGray, EForegroundColor::Black);
		Console.PrintCommandLog(70, 0, 40, 29);
		
		Console.PrintChar(X, Y, '@', EForegroundColor::LightYellow);

		Console.PrintChar(112, 1, 'X');
		Console.PrintChar(112, 2, 'Y');
		Console.PrintHString(114, 1, std::to_string(X).c_str());
		Console.PrintHString(114, 2, std::to_string(Y).c_str());

		Console.Render();
	}

	ThreadInput.join();
	return 0;
}