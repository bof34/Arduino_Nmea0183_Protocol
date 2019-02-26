#include "Oled_Manager.h"


//U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_DEV_0);// | U8G_I2C_OPT_FAST);
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK);

Oled_Manager::Oled_Manager()
{
	
}


Oled_Manager::~Oled_Manager()
{
}

void Oled_Manager::SetPortDataStatusList(Port_DataStatus _portDataStatusList[])
{
	portDataStatusList = _portDataStatusList;
}
void Oled_Manager::Init()
{
	u8g.setFont(u8g_font_unifont);
	u8g.setDefaultForegroundColor(); 
	TestScreen("");
}
void Oled_Manager::Init(Screen screen)
{
	SelectScreen(screen);
	Init();
}

void Oled_Manager::TestScreen(char label[])
{
	int w = u8g.getStrWidth((const char *)label);

	u8g.firstPage();
	do {
		u8g.drawStr((128 - w) / 2, 32, (const char *)label);
	} while (u8g.nextPage());
}
void Oled_Manager::TestScreen(Screen screen, char label[])
{
	SelectScreen(screen); 
	TestScreen(label);
}

void Oled_Manager::SelectScreen(Screen screen)
{
	if (screen == Screen1)
	{
		I2C_SLA = (0x3c * 2);
	}
	else if (screen == Screen2)
	{
		I2C_SLA = (0x3d * 2);
	}
}

void Oled_Manager::ShowGlobalMenu(Screen screen)
{
	SelectScreen(screen);
	ShowGlobalMenu();
}
void Oled_Manager::ShowGlobalMenu()
{
	u8g.setFont(u8g_font_profont10);
	u8g.firstPage();
	do {
		for (int i = 0; i < 3; i++)
		{
			Port_DataStatus pds = portDataStatusList[i];
			if (pds.Input && !pds.Output)
			{
				u8g.drawTriangle(2 + (i*42), 0, 40 + (i * 42), 0, 19 + (i * 42), 10);
			}
			else if (!pds.Input && pds.Output)
			{
				u8g.drawTriangle(2 + (i * 42), 10, 40 + (i * 42), 10, 19 + (i * 42), 0);
			}
			else if (pds.Input && pds.Output)
			{
				u8g.drawTriangle(2 + (i * 42), 5, 40 + (i * 42), 5, 19 + (i * 42), 10);
				u8g.drawTriangle(2 + (i * 42), 5, 40 + (i * 42), 5, 19 + (i * 42), 0);
			}
			if (pds.Active) { u8g.drawRFrame(2 + (i * 42), 11, 38, 10, 3); }
			int w = (i * 42) + (42-u8g.getStrWidth((const char *)pds.Name))/2;
			u8g.drawStr(w, 19, (const char *)pds.Name); 
			w = (i * 42) + (42 - u8g.getStrWidth((const char *)pds.DataType)) / 2;
			u8g.drawStr(w, 29, (const char *)pds.DataType);
		}
		for (int i = 0; i < 3; i++)
		{
			Port_DataStatus pds = portDataStatusList[i+3];
			if (pds.Input && !pds.Output)
			{
				u8g.drawTriangle(2 + (i * 42), 64, 40 + (i * 42), 64, 19 + (i * 42), 54);
			}
			else if (!pds.Input && pds.Output)
			{
				u8g.drawTriangle(2 + (i * 42), 64, 40 + (i * 42), 54, 19 + (i * 42), 64);
			}
			else if (pds.Input && pds.Output)
			{
				u8g.drawTriangle(2 + (i * 42), 59, 40 + (i * 42), 59, 19 + (i * 42), 64);
				u8g.drawTriangle(2 + (i * 42), 59, 40 + (i * 42), 59, 19 + (i * 42), 54);
			}
			if (pds.Active) { u8g.drawRFrame(2 + (i * 42), 43, 38, 10, 3); }
			int w = (i * 42) + (42 - u8g.getStrWidth((const char *)pds.Name)) / 2;
			u8g.drawStr(w, 51, (const char *)pds.Name);
			w = (i * 42) + (42 - u8g.getStrWidth((const char *)pds.DataType)) / 2;
			u8g.drawStr(w, 41, (const char *)pds.DataType);
		}
	} while (u8g.nextPage());
}

void Oled_Manager::ClearScreen()
{
	u8g.firstPage();
	do {
	} while (u8g.nextPage());
}
void Oled_Manager::ClearScreen(Screen screen)
{
	SelectScreen(screen);
	ClearScreen();
}

uint8_t Oled_Manager::GetAddress()
{
	return I2C_SLA;
}


void Oled_Manager::ChangeMenu(Menu menu)
{
	menuOps = menu == Ops;
}
void Oled_Manager::ChangeMenu()
{
	menuOps = !menuOps;
}
Oled_Manager::Menu Oled_Manager::GetMenu()
{
	if (menuOps)
	{
		return Ops;
	}
	else
	{
		return Config;
	}
}
