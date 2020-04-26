#include "SingleBufferedConsole.h"
#include <string>
#include <thread>
#include <atomic>

int main()
{
	SingleBufferedConsole console{ 130, 30, "Console", ECommandLinePosition::Bottom };
	console.setClearBackground(EBackgroundColor::Black);
	console.setDefaultForeground(EForegroundColor::White);

	std::atomic<short> x{};
	std::atomic<short> y{};
	std::thread threadInput
	{
		[&]()
		{
			while (true)
			{
				if (console.isTerminated()) break;

				if (console.hitKey())
				{
					if (console.isKeyHit(EArrowKeys::Right)) ++x;
					if (console.isKeyHit(EArrowKeys::Left)) --x;
					if (console.isKeyHit(EArrowKeys::Down)) ++y;
					if (console.isKeyHit(EArrowKeys::Up)) --y;
					if (console.isKeyHit(VK_RETURN))
					{
						if (console.readCommand())
						{
							if (console.isLastCommand("/quit"))
							{
								console.terminate();
							}
						}
					}
				}
			}
		}
	};

	while (true)
	{
		if (console.isTerminated()) break;

		console.clear();

		console.fillBox(5, 5, 7, 10, '~', EBackgroundColor::Cyan, EForegroundColor::White);

		console.printBox(0, 0, 70, 29, ' ', EBackgroundColor::DarkGray, EForegroundColor::Black);

		console.printBox(70, 0, 40, 29, ' ', EBackgroundColor::DarkGray, EForegroundColor::Black);
		console.printCommandLog(70, 0, 40, 29);
		
		console.printChar(x, y, '@', EForegroundColor::LightYellow);

		console.printChar(112, 1, 'X');
		console.printChar(112, 2, 'Y');
		console.printHorzString(114, 1, std::to_string(x).c_str());
		console.printHorzString(114, 2, std::to_string(y).c_str());

		console.render();
	}

	threadInput.join();
	return 0;
}