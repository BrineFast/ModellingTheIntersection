#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include <list>
#include "error.h"

using namespace std;
using namespace sf;

class Walker {
private:
	float Speed;
	Texture WalkerImg;

public:
	float CurrentWalkerSpeed, CurrentWalkerX, CurrentWalkerY;
	string Direction;
	string NextDirection;
	Sprite WalkerTexture;

	Walker(float s, float cwx, float cwy, string d, string nd, string WayToWalkerTexture) {
		CurrentWalkerX = cwx;
		CurrentWalkerY = cwy;
		Speed = s;
		CurrentWalkerSpeed = Speed;
		Direction = d;
		NextDirection = nd;
		WalkerImg.loadFromFile(WayToWalkerTexture);
		WalkerTexture.setTexture(WalkerImg);
		WalkerTexture.setPosition(CurrentWalkerX, CurrentWalkerY);
		WalkerTexture.setScale(0.3, 0.3);
	}

	~Walker() {}

	void Moving(float XSpeed, float YSpeed) {
		CurrentWalkerX += XSpeed / 10;
		CurrentWalkerY += YSpeed / 10;
		WalkerTexture.setPosition(CurrentWalkerX, CurrentWalkerY);
	}

	bool CheckDistance(vector <Walker*> Walkers) {
		for (int i = 0; i < Walkers.size(); i++) {
			if (WalkerTexture.getPosition().x - Walkers[i]->WalkerTexture.getPosition().x < 20 && Walkers[i]->WalkerTexture.getPosition() != WalkerTexture.getPosition()) {
				CurrentWalkerSpeed = 0;
				return true;
			}
		}
		CurrentWalkerSpeed = Speed;
		return false;
	}

};


class Car {
private:
	string Type;

public:
	int Mass;
	float CurrentCarX, CurrentCarY, MoveX, MoveY, MaxSpeed, CurrentSpeed;
	int Weight, Height;
	bool LineChanged;
	string WayAxis;
	string Direction, NextDirection;
	RectangleShape* CheckArea = new RectangleShape(Vector2f(10, 10));

	Texture CarImg;
	Sprite CarTexture;
	Car(float m, float ccx, float ccy, float s, string t, string d) {
		Mass = m;
		CurrentCarX = ccx;
		CurrentCarY = ccy;
		MaxSpeed = s;
		CurrentSpeed = MaxSpeed;
		Type = t;
		Direction = d;
		NextDirection = d;
		CheckArea->setFillColor(Color::Green);
		CheckArea->setScale(1, 6);
	}

	~Car() {};
	//Чтобы машина ехала ровно, меньшая скорость всегда 60% от большой скорости

	void Braking() {
		if (CurrentSpeed > 0)
			CurrentSpeed -= MaxSpeed * 0.01;
		else
			CurrentSpeed = 0;
	}

	//Каждые 20 км/ч = 1 секунда
	void Acceleration() {
		if (CurrentSpeed < MaxSpeed)
			CurrentSpeed += 0.0005;
	}

	void Moving(float XSpeed, float YSpeed) {
		CurrentCarX += XSpeed / 10;
		CurrentCarY += YSpeed / 10;
		CarTexture.setPosition(CurrentCarX, CurrentCarY);
		if (Direction == "HorizontalRight") {
			CheckArea->setRotation(125);
			CheckArea->setPosition(CarTexture.getPosition() + Vector2f(20, 20));
		}
		else if (Direction == "HorizontalLeft") {
			CheckArea->setRotation(-55);
			CheckArea->setPosition(CarTexture.getPosition() + Vector2f(90, 50));
		}
		else if (Direction == "VerticalUp") {
			CheckArea->setRotation(-125);
			CheckArea->setPosition(CarTexture.getPosition() + Vector2f(80, 40));
		}
		else {
			CheckArea->setRotation(52);
			CheckArea->setPosition(CarTexture.getPosition() + Vector2f(40, 60));
		}
	}

	void ChangeWay() {
		if (Direction == "HorizontalLeft") {
			int Rotate = rand() % 3 + 1;
			switch (Rotate) {
			case 1: //вверх
				NextDirection = "VerticalUp";
				break;
			case 2: //Вниз
				NextDirection = "VerticalDown";
				break;
			case 3:
				break;
			}
		}
		else if (Direction == "HorizontalRight") {
			int Rotate = rand() % 3 + 1;
			switch (Rotate) {
			case 1: //вверх
				NextDirection = "VerticalUp";
				break;
			case 2: //Вниз
				NextDirection = "VerticalDown";
				break;
			case 3:
				break;
			}
		}
		else if (Direction == "VerticalDown") {
			int Rotate = rand() % 3 + 1;
			switch (Rotate) {
			case 1: //влево
				NextDirection = "HorizontalLeft";
				break;
			case 2: //Вниз
				NextDirection = "HorizontalRight";
				break;
			case 3:
				break;
			}
		}
		else if (Direction == "VerticalUp") {
			int Rotate = rand() % 3 + 1;
			switch (Rotate) {
			case 1: //влево
				NextDirection = "HorizontalLeft";
				break;
			case 2: //Вниз
				NextDirection = "HorizontalRight";
				break;
			case 3:
				break;
			}
		}
	}

	void SetTexture(string WayToCarTexture) {
		CarImg.loadFromFile(WayToCarTexture);
		CarTexture.setTexture(CarImg);
		CarTexture.setPosition(CurrentCarX, CurrentCarY);
		CarTexture.setScale(0.3, 0.3);
	}

	bool IsOppositeDirections(string Dir) {
		if (Dir == "VerticalUp" && Direction == "VerticalDown")
			return true;
		else if (Dir == "HorizontalLeft" && Direction == "HorizontalRight")
			return true;
		return false;
	}
};

class PassengerCar : public Car {
public:
	PassengerCar(float m, float ccx, float ccy, float s, string t, string d, string WayToCarTexture) : Car(1500, ccx, ccy, s, "Легковая", d) {
		CarImg.loadFromFile(WayToCarTexture);
		CarTexture.setTexture(CarImg);
		CarTexture.setPosition(CurrentCarX, CurrentCarY);
		CarTexture.setScale(0.3, 0.3);
		LineChanged = false;
	}
	~PassengerCar() {};

	void CheckAheadCarSpeed(vector <PassengerCar*> Cars, vector <TrafficLight*> TraffficLights) {
		for (int i = 0; i < Cars.size(); i++) {
			if (CheckArea->getGlobalBounds().intersects(Cars[i]->CarTexture.getGlobalBounds()) && Cars[i]->Direction == Direction && Cars[i]->CarTexture.getGlobalBounds() != CarTexture.getGlobalBounds() && Cars[i]->LineChanged == LineChanged) {
				Cars[i]->CurrentSpeed * 0.9;
				Braking();
			}
			else if (CheckArea->getGlobalBounds().intersects(Cars[i]->CarTexture.getGlobalBounds()) && IsOppositeDirections(Cars[i]->Direction) == false && (Direction == "VerticalUp" || Direction == "VerticalDown") && TraffficLights[0]->CurrentSignal == "Green")
				Braking();
			else if (CheckArea->getGlobalBounds().intersects(Cars[i]->CarTexture.getGlobalBounds()) && IsOppositeDirections(Cars[i]->Direction) == false && (Direction == "HorizontalLeft" || Direction == "HorizontalRight") && TraffficLights[1]->CurrentSignal == "Green")
				Braking();
		}
	}

	void CheckWalker(vector <Walker*> Walkers) {
		for (int i = 0; i < Walkers.size(); i++)
			if (CheckArea->getGlobalBounds().intersects(Walkers[i]->WalkerTexture.getGlobalBounds()))
				Braking();
	}

	bool CarCrash(vector <PassengerCar*> Cars) {
		for (int i = 0; i < Cars.size(); i++)
			if (Cars[i]->CarTexture.getGlobalBounds().intersects(CarTexture.getGlobalBounds()))
				return true;
		return false;
	}
};

class TrafficLight {
public:
	string TrafficlightMode;
	int DynamicModeSwitchDelay;
	float SwitchDelay;
	float ViewDistance;
	list <Walker*> WalkersCount;
	list <PassengerCar*> CarsCount;
	string CurrentSignal;
	CircleShape* Signal = new CircleShape(8), * YellowSignal = new CircleShape(8);

	TrafficLight(string tm, float sd, float vd, string cs) {
		TrafficlightMode = tm;
		SwitchDelay = sd;
		DynamicModeSwitchDelay = sd;
		ViewDistance = vd;
		CurrentSignal = cs;
		Signal->setFillColor(Color::Transparent);
		YellowSignal->setFillColor(Color::Transparent);
	}

	bool SwitchSignalStatic(string Current, float SwitchTimerSeconds) {
		bool Swithced = false;
		YellowSignal->setFillColor(Color::Transparent);
		if (SwitchTimerSeconds >= SwitchDelay) {
			if (CurrentSignal == "Green") {
				Signal->setFillColor(Color::Red);
				Signal->setPosition(Signal->getPosition() - Vector2f(0, 47));
				CurrentSignal = "Red";
			}
			else if (CurrentSignal == "Red") {
				Signal->setFillColor(Color::Green);
				Signal->setPosition(Signal->getPosition() + Vector2f(0, 47));
				CurrentSignal = "Green";
			}
			Swithced = true;
		}
		return Swithced;
	}

	bool SwitchSignalDynamic(string Current) {
		bool Swithced = false;
		YellowSignal->setFillColor(Color::Transparent);
		if (CurrentSignal == "Green") {
			Signal->setFillColor(Color::Red);
			Signal->setPosition(Signal->getPosition() - Vector2f(0, 47));
			CurrentSignal = "Red";
			Swithced = true;
		}
		else if (CurrentSignal == "Red") {
			Signal->setFillColor(Color::Green);
			Signal->setPosition(Signal->getPosition() + Vector2f(0, 47));
			CurrentSignal = "Green";
			Swithced = true;
		}
		return Swithced;
	}



	void WalkersCounter(Walker* Walk) {
		if (CurrentSignal == "Red") {
			WalkersCount.push_back(Walk);
		}
		else
			WalkersCount.clear();
	}

	void CarsCounter(PassengerCar* Car) {
		if (CurrentSignal == "Red") {
			CarsCount.push_back(Car);
		}
		else
			CarsCount.clear();
	}

	void DynamicModeSwitchDelaySet(vector <TrafficLight*> TrafficLights) {
		TrafficLights[0]->CarsCount.unique();
		TrafficLights[1]->CarsCount.unique();
		TrafficLights[0]->WalkersCount.unique();
		TrafficLights[1]->WalkersCount.unique();
		if (TrafficLights[0]->CarsCount.size() + TrafficLights[0]->WalkersCount.size() > TrafficLights[0]->CarsCount.size() + TrafficLights[0]->WalkersCount.size())
			DynamicModeSwitchDelay = TrafficLights[0]->CarsCount.size() * 2 + TrafficLights[0]->WalkersCount.size();
		else
			DynamicModeSwitchDelay = TrafficLights[1]->CarsCount.size() * 2 + TrafficLights[1]->WalkersCount.size();
	}

};

Font setFont(string fname) {
	Font font;
	try {
		bool isOk = font.loadFromFile(fname);
		if (!isOk)
			throw(fname);
	}
	catch (...) {
		cerr << "Файл, содержащий шрифт " << fname << ", не найден";
	}
	return font;
}

void StartColor(list <RectangleShape*> Buttons) {
	for (auto i : Buttons)
		i->setFillColor(Color(50, 50, 50, 255));
}

bool CheckIfSpawnClear(string Direction, vector <RectangleShape*> CheckSpawnAreas, vector <PassengerCar*> Cars) {
	for (int i = 0; i < Cars.size(); i++) {
		if (Direction == "VerticalUp" && Cars[i]->Direction == Direction && CheckSpawnAreas[0]->getGlobalBounds().intersects(Cars[i]->CarTexture.getGlobalBounds()))
			return false;
		else if (Direction == "HorizontalRight" && Cars[i]->Direction == Direction && CheckSpawnAreas[1]->getGlobalBounds().intersects(Cars[i]->CarTexture.getGlobalBounds()))
			return false;
		else if (Direction == "VerticalDown" && Cars[i]->Direction == Direction && CheckSpawnAreas[2]->getGlobalBounds().intersects(Cars[i]->CarTexture.getGlobalBounds()))
			return false;
		else if (Direction == "HorizontalLeft" && Cars[i]->Direction == Direction && CheckSpawnAreas[3]->getGlobalBounds().intersects(Cars[i]->CarTexture.getGlobalBounds()))
			return false;
	}
	return true;
}

int main() {
	setlocale(LC_ALL, "RUS");
	RenderWindow window(VideoMode(1280, 720), L"Моделирование движения на перекрестке");
	window.setFramerateLimit(6000);

	//Переменные для отображения по умолчанию
	int SpeedLowerLimit = 50;
	int SpeedUpperLimit = 100;

	int TrafficLightsTiming = 30;
	string TrafficLightsMode = "clear";
	int TraFficLightsViewDistance = 10;

	int CarsSpawnRateY = 4;
	int CarsSpawnRateX = 4;
	int WalkersSpawnRate = 30;


	//Переменные статистики
	int CarCrashesCount = 0;

	float AverageCountOfCars = 0;
	float AverageSpeedOfCars = 0;

	int SummaryCarsCount = 0;

	float RealTimeOfModelling = 0;
	float ModelTime = 0;

	//Создание заднего фона меню
	Texture MenuBackground;
	MenuBackground.loadFromFile("Texture/bg_menu.jpg");
	Sprite MenuBack(MenuBackground);

	Texture ModelBackground;
	ModelBackground.loadFromFile("Texture/bg_model.jpg");
	Sprite ModelBack(ModelBackground);
	ModelBack.setScale(0.7f, 1.0f);

	Vector2f* BackgroundSize = new Vector2f(1280, 720);
	RectangleShape* Background = new RectangleShape(*BackgroundSize);
	Background->setFillColor(Color(255, 236, 194));

	string fontName = "Furore.ttf";
	Font font = setFont(fontName);

	Texture InModelStats;
	InModelStats.loadFromFile("Texture/model_stat.jpg");
	Sprite ModelStat(InModelStats);
	ModelStat.setPosition(1000, 0);


	//Создание элементов экрана настроек
	list <RectangleShape*> Buttons;

	//Кнопка моделирования
	RectangleShape* ButtonStartModel = new RectangleShape(Vector2f(200.0, 75.0));
	ButtonStartModel->setPosition(Vector2f(540.0, 600.0));
	ButtonStartModel->setFillColor(Color(50, 50, 50, 255));
	Buttons.push_back(ButtonStartModel);
	Text* TextButtonStartModel = new Text(L"Начать", font, 27);
	TextButtonStartModel->setPosition(585.0, 620.0);
	TextButtonStartModel->setFillColor(Color(179, 179, 179));


	//Настройка диапазона скоростей

	Text* TextSpeedAnonceFrom = new Text(L"Скорость от", font, 27);
	TextSpeedAnonceFrom->setPosition(60.0, 90.0);
	TextSpeedAnonceFrom->setFillColor(Color(240, 240, 240));
	Text* TextSpeedLowerLimit = new Text(to_string(SpeedLowerLimit), font, 27);
	TextSpeedLowerLimit->setPosition(275.0, 90.0);
	TextSpeedLowerLimit->setFillColor(Color(239, 222, 205));

	Text* TextSpeedAnonceTo = new Text(L"до", font, 27);
	TextSpeedAnonceTo->setPosition(330.0, 90.0);
	TextSpeedAnonceTo->setFillColor(Color(240, 240, 240));
	Text* TextSpeedUpperLimit = new Text(to_string(SpeedUpperLimit), font, 27);
	TextSpeedUpperLimit->setPosition(385.0, 90.0);
	TextSpeedUpperLimit->setFillColor(Color(239, 222, 205));

	//Кнопка "+" от
	RectangleShape* ButtonPlusSpeedFrom = new RectangleShape(Vector2f(40.0, 40.0));
	ButtonPlusSpeedFrom->setPosition(Vector2f(278.0, 35.0));
	ButtonPlusSpeedFrom->setFillColor(Color(50, 50, 50, 255));
	Buttons.push_back(ButtonPlusSpeedFrom);

	Text* TextButtonPlusSpeedFrom = new Text("+", font, 30);
	TextButtonPlusSpeedFrom->setPosition(289.0, 35.0);
	TextButtonPlusSpeedFrom->setFillColor(Color(179, 179, 179));

	//Кнопка "-" от
	RectangleShape* ButtonMinusSpeedFrom = new RectangleShape(Vector2f(40.0, 40.0));
	ButtonMinusSpeedFrom->setPosition(Vector2f(278.0, 135.0));
	ButtonMinusSpeedFrom->setFillColor(Color(50, 50, 50, 255));
	Buttons.push_back(ButtonMinusSpeedFrom);

	Text* TextButtonMinusSpeedFrom = new Text("-", font, 30);
	TextButtonMinusSpeedFrom->setPosition(293.0, 135.0);
	TextButtonMinusSpeedFrom->setFillColor(Color(179, 179, 179));

	//Кнопка "+" до
	RectangleShape* ButtonPlusSpeedTo = new RectangleShape(Vector2f(40.0, 40.0));
	ButtonPlusSpeedTo->setPosition(Vector2f(387.0, 35.0));
	ButtonPlusSpeedTo->setFillColor(Color(50, 50, 50, 255));
	Buttons.push_back(ButtonPlusSpeedTo);

	Text* TextButtonPlusSpeedTo = new Text("+", font, 30);
	TextButtonPlusSpeedTo->setPosition(398.0, 35.0);
	TextButtonPlusSpeedTo->setFillColor(Color(179, 179, 179));

	//Кнопка "-" до
	RectangleShape* ButtonMinusSpeedTo = new RectangleShape(Vector2f(40.0, 40.0));
	ButtonMinusSpeedTo->setPosition(Vector2f(387.0, 135.0));
	ButtonMinusSpeedTo->setFillColor(Color(50, 50, 50, 255));
	Buttons.push_back(ButtonMinusSpeedTo);

	Text* TextButtonMinusSpeedTo = new Text("-", font, 30);
	TextButtonMinusSpeedTo->setPosition(402.0, 135.0);
	TextButtonMinusSpeedTo->setFillColor(Color(179, 179, 179));


	//Настройки дальности видимости светофора

	Text* TextViewDistance = new Text(L"Дальность \nвидимости \nсветофора", font, 27);
	TextViewDistance->setPosition(60.0, 240.0);
	TextViewDistance->setFillColor(Color(240, 240, 240));

	Text* TextTraFficLightsViewDistance = new Text(to_string(TraFficLightsViewDistance), font, 27);
	TextTraFficLightsViewDistance->setPosition(282.0, 272.0);
	TextTraFficLightsViewDistance->setFillColor(Color(239, 222, 205));

	//Кнопка "+"
	RectangleShape* ButtonViewDistancePlus = new RectangleShape(Vector2f(40.0, 40.0));
	ButtonViewDistancePlus->setPosition(Vector2f(278.0, 220.0));
	ButtonViewDistancePlus->setFillColor(Color(50, 50, 50, 255));
	Buttons.push_back(ButtonViewDistancePlus);

	Text* TextButtonViewDistancePlus = new Text("+", font, 30);
	TextButtonViewDistancePlus->setPosition(289.0, 220.0);
	TextButtonViewDistancePlus->setFillColor(Color(179, 179, 179));

	//Кнопка "-"
	RectangleShape* ButtonViewDistanceMinus = new RectangleShape(Vector2f(40.0, 40.0));
	ButtonViewDistanceMinus->setPosition(Vector2f(278.0, 320.0));
	ButtonViewDistanceMinus->setFillColor(Color(50, 50, 50, 255));
	Buttons.push_back(ButtonViewDistanceMinus);

	Text* TextButtonViewDistanceMinus = new Text("-", font, 30);
	TextButtonViewDistanceMinus->setPosition(293.0, 320.0);
	TextButtonViewDistanceMinus->setFillColor(Color(179, 179, 179));


	//Настройка спавнрейта автомобилей
	//Дорога по оси Y

	Text* TextCarsSpawnRateY = new Text(L"Наибольший интервал\nпоявления машин по\nвертикальной дороге", font, 27);
	TextCarsSpawnRateY->setPosition(800.0, 75.0);
	TextCarsSpawnRateY->setFillColor(Color(240, 240, 240));
	Text* TextCarsSpawnRateYIndicator = new Text(to_string(CarsSpawnRateY), font, 27);
	TextCarsSpawnRateYIndicator->setPosition(1186.0, 90.0);
	TextCarsSpawnRateYIndicator->setFillColor(Color(239, 222, 205));

	//Кнопка "+"
	RectangleShape* ButtonCarsSpawnRateYPlus = new RectangleShape(Vector2f(40.0, 40.0));
	ButtonCarsSpawnRateYPlus->setPosition(Vector2f(1180.0, 35.0));
	ButtonCarsSpawnRateYPlus->setFillColor(Color(50, 50, 50, 255));
	Buttons.push_back(ButtonCarsSpawnRateYPlus);

	Text* TextButtonCarsSpawnRateYPlus = new Text("+", font, 30);
	TextButtonCarsSpawnRateYPlus->setPosition(1191.0, 35.0);
	TextButtonCarsSpawnRateYPlus->setFillColor(Color(179, 179, 179));

	//Кнопка "-"
	RectangleShape* ButtonCarsSpawnRateYMinus = new RectangleShape(Vector2f(40.0, 40.0));
	ButtonCarsSpawnRateYMinus->setPosition(Vector2f(1180.0, 135.0));
	ButtonCarsSpawnRateYMinus->setFillColor(Color(50, 50, 50, 255));
	Buttons.push_back(ButtonCarsSpawnRateYMinus);

	Text* TextButtonCarsSpawnRateYMinus = new Text("-", font, 30);
	TextButtonCarsSpawnRateYMinus->setPosition(1195.0, 135.0);
	TextButtonCarsSpawnRateYMinus->setFillColor(Color(179, 179, 179));

	//Дорога по оси X
	Text* TextCarsSpawnRateX = new Text(L"По горизонтальной", font, 27);
	TextCarsSpawnRateX->setPosition(837.0, 272.0);
	TextCarsSpawnRateX->setFillColor(Color(240, 240, 240));

	Text* TextCarsSpawnRateXIndicator = new Text(to_string(CarsSpawnRateX), font, 27);
	TextCarsSpawnRateXIndicator->setPosition(1186.0, 272.0);
	TextCarsSpawnRateXIndicator->setFillColor(Color(239, 222, 205));

	//Кнопка "+"
	RectangleShape* ButtonCarsSpawnRateXPlus = new RectangleShape(Vector2f(40.0, 40.0));
	ButtonCarsSpawnRateXPlus->setPosition(Vector2f(1180.0, 220.0));
	ButtonCarsSpawnRateXPlus->setFillColor(Color(50, 50, 50, 255));
	Buttons.push_back(ButtonCarsSpawnRateXPlus);

	Text* TextButtonCarsSpawnRateXPlus = new Text("+", font, 30);
	TextButtonCarsSpawnRateXPlus->setPosition(1191.0, 220.0);
	TextButtonCarsSpawnRateXPlus->setFillColor(Color(179, 179, 179));

	//Кнопка "-"
	RectangleShape* ButtonCarsSpawnRateXMinus = new RectangleShape(Vector2f(40.0, 40.0));
	ButtonCarsSpawnRateXMinus->setPosition(Vector2f(1180.0, 320.0));
	ButtonCarsSpawnRateXMinus->setFillColor(Color(50, 50, 50, 255));
	Buttons.push_back(ButtonCarsSpawnRateXMinus);

	Text* TextButtonCarsSpawnRateXMinus = new Text("-", font, 30);
	TextButtonCarsSpawnRateXMinus->setPosition(1195.0, 320.0);
	TextButtonCarsSpawnRateXMinus->setFillColor(Color(179, 179, 179));


	//Выбор режима работы светофора
	Text* TextTrafficLightsMode = new Text(L"Режим светофора", font, 27);
	TextTrafficLightsMode->setPosition(500.0, 490.0);
	TextTrafficLightsMode->setFillColor(Color(240, 240, 240));

	//Статический
	RectangleShape* ButtonTrafficLightsModeStatic = new RectangleShape(Vector2f(260.0, 75.0));
	ButtonTrafficLightsModeStatic->setPosition(Vector2f(378.0, 525.0));
	ButtonTrafficLightsModeStatic->setFillColor(Color(50, 50, 50, 255));
	Buttons.push_back(ButtonTrafficLightsModeStatic);

	Text* TextButtonTrafficLightsModeStatic = new Text(L"Статический", font, 27);
	TextButtonTrafficLightsModeStatic->setPosition(402.0, 545.0);
	TextButtonTrafficLightsModeStatic->setFillColor(Color(179, 179, 179));

	//Динамический
	RectangleShape* ButtonTrafficLightsModeDynamic = new RectangleShape(Vector2f(260.0, 75.0));
	ButtonTrafficLightsModeDynamic->setPosition(Vector2f(638.0, 525.0));
	ButtonTrafficLightsModeDynamic->setFillColor(Color(50, 50, 50, 255));
	Buttons.push_back(ButtonTrafficLightsModeDynamic);

	Text* TextButtonTrafficLightsModeDynamic = new Text(L"Динамический", font, 27);
	TextButtonTrafficLightsModeDynamic->setPosition(651.0, 545.0);
	TextButtonTrafficLightsModeDynamic->setFillColor(Color(179, 179, 179));

	//Настройка спавнрейта пешеходов
	Text* TextWalkersSpawnRate = new Text(L"Пешеходов \nв минуту", font, 27);
	TextWalkersSpawnRate->setPosition(60.0, 435.0);
	TextWalkersSpawnRate->setFillColor(Color(240, 240, 240));

	Text* TextWalkersSpawnRateIndicator = new Text(to_string(WalkersSpawnRate), font, 27);
	TextWalkersSpawnRateIndicator->setPosition(285.0, 450.0);
	TextWalkersSpawnRateIndicator->setFillColor(Color(239, 222, 205));

	//Кнопка "+"
	RectangleShape* ButtonWalkersSpawnRatePlus = new RectangleShape(Vector2f(40.0, 40.0));
	ButtonWalkersSpawnRatePlus->setPosition(Vector2f(278.0, 395.0));
	ButtonWalkersSpawnRatePlus->setFillColor(Color(50, 50, 50, 255));
	Buttons.push_back(ButtonWalkersSpawnRatePlus);

	Text* TextButtonWalkersSpawnRatePlus = new Text("+", font, 30);
	TextButtonWalkersSpawnRatePlus->setPosition(289.0, 395.0);
	TextButtonWalkersSpawnRatePlus->setFillColor(Color(179, 179, 179));

	//Кнопка "-"
	RectangleShape* ButtonWalkersSpawnRateMinus = new RectangleShape(Vector2f(40.0, 40.0));
	ButtonWalkersSpawnRateMinus->setPosition(Vector2f(278.0, 495.0));
	ButtonWalkersSpawnRateMinus->setFillColor(Color(50, 50, 50, 255));
	Buttons.push_back(ButtonWalkersSpawnRateMinus);

	Text* TextButtonWalkersSpawnRateMinus = new Text("-", font, 30);
	TextButtonWalkersSpawnRateMinus->setPosition(293.0, 495.0);
	TextButtonWalkersSpawnRateMinus->setFillColor(Color(179, 179, 179));


	//Тайминги светофора
	Text* TextTrafficLightsTiming = new Text(L"Период \nсмены сигнала", font, 27);
	TextTrafficLightsTiming->setPosition(902.0, 435.0);
	TextTrafficLightsTiming->setFillColor(Color(240, 240, 240));

	Text* TextTrafficLightsTimingIndicator = new Text(to_string(TrafficLightsTiming), font, 27);
	TextTrafficLightsTimingIndicator->setPosition(1180.0, 450.0);
	TextTrafficLightsTimingIndicator->setFillColor(Color(239, 222, 205));

	//Кнопка "+"
	RectangleShape* ButtonTrafficLightsTimingUp = new RectangleShape(Vector2f(40.0, 40.0));
	ButtonTrafficLightsTimingUp->setPosition(Vector2f(1180.0, 395.0));
	ButtonTrafficLightsTimingUp->setFillColor(Color(50, 50, 50, 255));
	Buttons.push_back(ButtonTrafficLightsTimingUp);

	Text* TextButtonTrafficLightsTimingUp = new Text("+", font, 30);
	TextButtonTrafficLightsTimingUp->setPosition(1191.0, 395.0);
	TextButtonTrafficLightsTimingUp->setFillColor(Color(179, 179, 179));

	//Кнопка "-"
	RectangleShape* ButtonTrafficLightsTimingDown = new RectangleShape(Vector2f(40.0, 40.0));
	ButtonTrafficLightsTimingDown->setPosition(Vector2f(1180.0, 495.0));
	ButtonTrafficLightsTimingDown->setFillColor(Color(50, 50, 50, 255));
	Buttons.push_back(ButtonTrafficLightsTimingDown);

	Text* TextButtonTrafficLightsTimingDown = new Text("-", font, 30);
	TextButtonTrafficLightsTimingDown->setPosition(1195.0, 495.0);
	TextButtonTrafficLightsTimingDown->setFillColor(Color(179, 179, 179));



	//Логика моделирования
	int SwitchScreen = 0; //Переход из окна настройки в моделирование

	//Машины
	vector <PassengerCar*> Cars;

	//Пешеходы
	vector <Walker*> Walkers;

	//Таймеры
	Clock SpawnTimerCarX;
	Clock SpawnTimerCarY;
	Clock RealTime;
	Clock WalkersSpawnTimer;

	//Светофор
	Clock SwitchTimer;
	vector <TrafficLight*> TrafficLights;
	vector <RectangleShape*> CheckTrafficlightsAreas;
	vector <RectangleShape*> CheckRotateAreas;
	vector <RectangleShape*> CheckLineChangeAreas;
	vector <RectangleShape*> CheckSpawnAreas;

	while (window.isOpen()) {
		switch (SwitchScreen) {
			Event event;
		case 0: {
			StartColor(Buttons);
			while (window.pollEvent(event))
			{
				if (event.type == Event::Closed)
					window.close();
				if (event.type == Event::MouseButtonPressed) {
					try {
						if (IntRect(278, 35, 40, 40).contains(Mouse::getPosition(window))) {
							if (SpeedLowerLimit + 10 > 100) throw InputException("Скорость не может быть больше 100 км/ч");
							SpeedLowerLimit += 10;
							TextSpeedLowerLimit->setString(to_string(SpeedLowerLimit));
							ButtonPlusSpeedFrom->setFillColor(Color::Black);
						}
						else if (IntRect(278, 135, 40, 40).contains(Mouse::getPosition(window))) {
							if (SpeedLowerLimit - 10 < 50) throw InputException("Скорость не может быть меньше 50 км/ч");
							SpeedLowerLimit -= 10;
							TextSpeedLowerLimit->setString(to_string(SpeedLowerLimit));
							ButtonMinusSpeedFrom->setFillColor(Color::Black);
						}
						else if (IntRect(387, 35, 40, 40).contains(Mouse::getPosition(window))) {
							if (SpeedUpperLimit + 10 > 100) throw InputException("Скорость не может быть больше 100 км/ч");
							SpeedUpperLimit += 10;
							TextSpeedUpperLimit->setString(to_string(SpeedUpperLimit));
							ButtonPlusSpeedTo->setFillColor(Color::Black);
						}
						else if (IntRect(387, 135, 40, 40).contains(Mouse::getPosition(window))) {
							if (SpeedUpperLimit - 10 < 50) throw InputException("Скорость не может быть меньше 50 км/ч");
							SpeedUpperLimit -= 10;
							TextSpeedUpperLimit->setString(to_string(SpeedUpperLimit));
							ButtonMinusSpeedTo->setFillColor(Color::Black);
						}
						else if (IntRect(278, 220, 40, 40).contains(Mouse::getPosition(window))) {
							if (TraFficLightsViewDistance + 10 > 50) throw InputException("Дальность видимости светофора не может быть больше 50 метров");
							TraFficLightsViewDistance += 10;
							TextTraFficLightsViewDistance->setString(to_string(TraFficLightsViewDistance));
							ButtonViewDistancePlus->setFillColor(Color::Black);
						}
						else if (IntRect(278, 320, 40, 40).contains(Mouse::getPosition(window))) {
							if (TraFficLightsViewDistance - 10 < 10) throw InputException("Дальность видимости светофора не может быть меньше 10 метров");
							TraFficLightsViewDistance -= 10;
							TextTraFficLightsViewDistance->setString(to_string(TraFficLightsViewDistance));
							ButtonViewDistanceMinus->setFillColor(Color::Black);
						}
						else if (IntRect(278, 395, 40, 40).contains(Mouse::getPosition(window))) {
							if (WalkersSpawnRate + 1 > 20) throw InputException("Количество пешеходов не может быть более 20 в минуту");
							WalkersSpawnRate += 1;
							TextWalkersSpawnRateIndicator->setString(to_string(WalkersSpawnRate));
							ButtonWalkersSpawnRatePlus->setFillColor(Color::Black);
						}
						else if (IntRect(278, 495, 40, 40).contains(Mouse::getPosition(window))) {
							if (WalkersSpawnRate - 1 < 10) throw InputException("Количество пешеходов не может быть меньше 10 в минуту");
							WalkersSpawnRate -= 1;
							TextWalkersSpawnRateIndicator->setString(to_string(WalkersSpawnRate));
							ButtonWalkersSpawnRateMinus->setFillColor(Color::Black);
						}
						else if (IntRect(1180, 35, 40, 40).contains(Mouse::getPosition(window))) {
							if (CarsSpawnRateY + 1 > 10) throw InputException("Количество проезжающих в минуту машин по вертикальной дороге не может быть более 10");
							CarsSpawnRateY += 1;
							TextCarsSpawnRateYIndicator->setString(to_string(CarsSpawnRateY));
							ButtonCarsSpawnRateYPlus->setFillColor(Color::Black);
						}
						else if (IntRect(1180, 135, 40, 40).contains(Mouse::getPosition(window))) {
							if (CarsSpawnRateY - 1 < 4) throw InputException("Количество проезжающих в минуту машин по вертикальной дороге не может быть менее 4");
							CarsSpawnRateY -= 1;
							TextCarsSpawnRateYIndicator->setString(to_string(CarsSpawnRateY));
							ButtonCarsSpawnRateYMinus->setFillColor(Color::Black);
						}
						else if (IntRect(1180, 220, 40, 40).contains(Mouse::getPosition(window))) {
							if (CarsSpawnRateX + 1 > 10) throw InputException("Количество проезжающих в минуту машин по горизонтальной дороге не может быть более 10");
							CarsSpawnRateX += 1;
							TextCarsSpawnRateXIndicator->setString(to_string(CarsSpawnRateX));
							ButtonCarsSpawnRateXPlus->setFillColor(Color::Black);
						}
						else if (IntRect(1180, 320, 40, 40).contains(Mouse::getPosition(window))) {
							if (CarsSpawnRateX - 1 < 4) throw InputException("Количество проезжающих в минуту машин по горизонтальной дороге не может быть менее 4");
							CarsSpawnRateX -= 1;
							TextCarsSpawnRateXIndicator->setString(to_string(CarsSpawnRateX));
							ButtonCarsSpawnRateXMinus->setFillColor(Color::Black);
						}
						else if (IntRect(1180, 395, 40, 40).contains(Mouse::getPosition(window))) {
							if (TrafficLightsTiming + 5 > 60) throw InputException("Сигнал светофора не может меняться реже, чем каждую минуту");
							TrafficLightsTiming += 5;
							TextTrafficLightsTimingIndicator->setString(to_string(TrafficLightsTiming));
							ButtonTrafficLightsTimingUp->setFillColor(Color::Black);
						}
						else if (IntRect(1180, 495, 40, 40).contains(Mouse::getPosition(window))) {
							if (TrafficLightsTiming - 5 < 10) throw InputException("Сигнал светофора не может меняться чаще, чем каждые 10 секунд");
							TrafficLightsTiming -= 5;
							TextTrafficLightsTimingIndicator->setString(to_string(TrafficLightsTiming));
							ButtonTrafficLightsTimingDown->setFillColor(Color::Black);
						}
						else if (IntRect(378, 525, 260, 75).contains(Mouse::getPosition(window))) {
							if (TrafficLightsMode == "Static") throw InputException("Статический режим светофора уже выбран");
							TrafficLightsMode = "Static";
							TextButtonTrafficLightsModeStatic->setFillColor(Color::Green);
							TextButtonTrafficLightsModeDynamic->setFillColor(Color(179, 179, 179));
							ButtonTrafficLightsModeStatic->setFillColor(Color::Black);
						}
						else if (IntRect(638, 545, 260, 75).contains(Mouse::getPosition(window))) {
							if (TrafficLightsMode == "Dynamic") throw InputException("Динамический режим светофора уже выбран");
							TrafficLightsMode = "Dynamic";
							TextButtonTrafficLightsModeDynamic->setFillColor(Color::Green);
							TextButtonTrafficLightsModeStatic->setFillColor(Color(179, 179, 179));
							ButtonTrafficLightsModeDynamic->setFillColor(Color::Black);
						}
						else if (IntRect(540, 600, 260, 75).contains(Mouse::getPosition(window))) {
							if (TrafficLightsMode == "clear") throw Exception("Не выбран тип работы светофора!");
							ButtonStartModel->setFillColor(Color::Black);

							//Создание светофоров
							if (TrafficLightsMode == "Static") {
								TrafficLights.push_back(new TrafficLight(TrafficLightsMode, TrafficLightsTiming, TraFficLightsViewDistance, "Green"));
								TrafficLights[0]->Signal->setFillColor(Color::Green);
								TrafficLights[0]->Signal->setPosition(339, 240);
								TrafficLights.push_back(new TrafficLight(TrafficLightsMode, TrafficLightsTiming, TraFficLightsViewDistance, "Red"));
								TrafficLights[1]->Signal->setFillColor(Color::Red);
								TrafficLights[1]->Signal->setPosition(540, 320);
								TrafficLights.push_back(new TrafficLight(TrafficLightsMode, TrafficLightsTiming, TraFficLightsViewDistance, "Green"));
								TrafficLights[2]->Signal->setFillColor(Color::Green);
								TrafficLights[2]->Signal->setPosition(773, 240);
								TrafficLights.push_back(new TrafficLight(TrafficLightsMode, TrafficLightsTiming, TraFficLightsViewDistance, "Red"));
								TrafficLights[3]->Signal->setFillColor(Color::Red);
								TrafficLights[3]->Signal->setPosition(538, 54);
							}
							else {
								TrafficLights.push_back(new TrafficLight(TrafficLightsMode, 5, TraFficLightsViewDistance, "Green"));
								TrafficLights[0]->Signal->setFillColor(Color::Green);
								TrafficLights[0]->Signal->setPosition(339, 240);
								TrafficLights.push_back(new TrafficLight(TrafficLightsMode, 5, TraFficLightsViewDistance, "Red"));
								TrafficLights[1]->Signal->setFillColor(Color::Red);
								TrafficLights[1]->Signal->setPosition(540, 320);
								TrafficLights.push_back(new TrafficLight(TrafficLightsMode, 5, TraFficLightsViewDistance, "Green"));
								TrafficLights[2]->Signal->setFillColor(Color::Green);
								TrafficLights[2]->Signal->setPosition(773, 240);
								TrafficLights.push_back(new TrafficLight(TrafficLightsMode, 5, TraFficLightsViewDistance, "Red"));
								TrafficLights[3]->Signal->setFillColor(Color::Red);
								TrafficLights[3]->Signal->setPosition(538, 54);
							}

							//Создание областей для считывая светофора
							// 0 - Снизу; 1 - справа; 2 - сверху; 3 - слева

							for (int i = 0; i < 4; i++)
								CheckTrafficlightsAreas.push_back(new RectangleShape(Vector2f(10, 10)));

							CheckTrafficlightsAreas[0]->setPosition(380, 480);
							CheckTrafficlightsAreas[0]->setScale(10, 20);
							CheckTrafficlightsAreas[0]->setRotation(56);

							CheckTrafficlightsAreas[1]->setPosition(750, 490);
							CheckTrafficlightsAreas[1]->setScale(10, 20);
							CheckTrafficlightsAreas[1]->setRotation(-59);

							CheckTrafficlightsAreas[2]->setPosition(725, 275);
							CheckTrafficlightsAreas[2]->setScale(11, 20);
							CheckTrafficlightsAreas[2]->setRotation(238);

							CheckTrafficlightsAreas[3]->setPosition(360, 265);
							CheckTrafficlightsAreas[3]->setScale(10, 20);
							CheckTrafficlightsAreas[3]->setRotation(122);

							for (int i = 0; i < 4; i++)
								CheckRotateAreas.push_back(new RectangleShape(Vector2f(10, 10)));

							//Поворот влево машин, которые едут сверху вниз
							//Поворот вниз машин, которые едут справа налево
							CheckRotateAreas[0]->setPosition(430, 270);
							CheckRotateAreas[0]->setScale(6, 6);
							CheckRotateAreas[0]->setFillColor(Color::Green);

							//Поворот вверх машин, которые едут справа налево
							//Поворот влево машин, которые едут снизу вверх
							CheckRotateAreas[1]->setPosition(555, 247);
							CheckRotateAreas[1]->setScale(8.5, 8.5);
							CheckRotateAreas[1]->setFillColor(Color::Green);

							//Поворот вправо машин, которые едут снизу вверх
							//Поворот вверх машин, которые едут слева направо
							CheckRotateAreas[2]->setPosition(615, 320);
							CheckRotateAreas[2]->setScale(6, 6);
							CheckRotateAreas[2]->setFillColor(Color::Green);


							//Поворот вниз машин, которые едут слева направо
							//Поворот вправо машин, которые едут сверху вниз
							CheckRotateAreas[3]->setPosition(515, 400);
							CheckRotateAreas[3]->setScale(5, 5);
							CheckRotateAreas[3]->setFillColor(Color::Green);

							for (int i = 0; i < 4; i++)
								CheckLineChangeAreas.push_back(new RectangleShape(Vector2f(10, 10)));

							CheckLineChangeAreas[0]->setPosition(580, 530);
							CheckLineChangeAreas[0]->setRotation(60);
							CheckLineChangeAreas[0]->setScale(10, 40);
							CheckLineChangeAreas[0]->setFillColor(Color::Green);

							CheckLineChangeAreas[1]->setPosition(810, 300);
							CheckLineChangeAreas[1]->setRotation(-57);
							CheckLineChangeAreas[1]->setScale(10, 30);
							CheckLineChangeAreas[1]->setFillColor(Color::Green);

							CheckLineChangeAreas[2]->setPosition(450, 200);
							CheckLineChangeAreas[2]->setRotation(237);
							CheckLineChangeAreas[2]->setScale(10, 40);
							CheckLineChangeAreas[2]->setFillColor(Color::Green);

							CheckLineChangeAreas[3]->setPosition(260, 400);
							CheckLineChangeAreas[3]->setRotation(125);
							CheckLineChangeAreas[3]->setScale(10, 40);
							CheckLineChangeAreas[3]->setFillColor(Color::Green);

							for (int i = 0; i < 4; i++)
								CheckSpawnAreas.push_back(new RectangleShape(Vector2f(10, 10)));

							CheckSpawnAreas[0]->setPosition(-30, 520);
							CheckSpawnAreas[0]->setScale(20, 20);
							CheckSpawnAreas[0]->setFillColor(Color::Green);

							CheckSpawnAreas[1]->setPosition(950, 500);
							CheckSpawnAreas[1]->setScale(22, 22);
							CheckSpawnAreas[1]->setFillColor(Color::Green);

							CheckSpawnAreas[2]->setPosition(900, 0);
							CheckSpawnAreas[2]->setScale(15, 15);
							CheckSpawnAreas[2]->setFillColor(Color::Green);

							CheckSpawnAreas[3]->setPosition(-60, 0);
							CheckSpawnAreas[3]->setScale(16, 16);
							CheckSpawnAreas[3]->setFillColor(Color::Green);

							SpawnTimerCarX.restart();
							SpawnTimerCarY.restart();
							RealTime.restart();
							WalkersSpawnTimer.restart();

							SwitchScreen = 1;
						}
					}
					catch (Exception) {
						continue;
					}
					catch (InputException) {
						continue;
					}
					catch (...) {
						cerr << "Неизвестная ошибка\n";
						continue;
					}
				}

				window.clear();
				window.draw(MenuBack);

				//Кнопки меню-настройки
				window.draw(*ButtonStartModel);
				window.draw(*TextButtonStartModel);

				window.draw(*TextSpeedLowerLimit);
				window.draw(*TextSpeedAnonceFrom);

				window.draw(*TextSpeedAnonceTo);
				window.draw(*ButtonPlusSpeedFrom);
				window.draw(*TextButtonPlusSpeedFrom);

				window.draw(*TextSpeedUpperLimit);
				window.draw(*ButtonPlusSpeedTo);
				window.draw(*TextButtonPlusSpeedTo);

				window.draw(*ButtonMinusSpeedTo);
				window.draw(*TextButtonMinusSpeedTo);

				window.draw(*ButtonMinusSpeedFrom);
				window.draw(*TextButtonMinusSpeedFrom);

				window.draw(*TextTraFficLightsViewDistance);
				window.draw(*TextViewDistance);
				window.draw(*ButtonViewDistanceMinus);
				window.draw(*TextButtonViewDistanceMinus);

				window.draw(*ButtonViewDistancePlus);
				window.draw(*TextButtonViewDistancePlus);

				window.draw(*TextCarsSpawnRateYIndicator);
				window.draw(*TextCarsSpawnRateY);
				window.draw(*ButtonCarsSpawnRateYMinus);
				window.draw(*TextButtonCarsSpawnRateYMinus);

				window.draw(*ButtonCarsSpawnRateYPlus);
				window.draw(*TextButtonCarsSpawnRateYPlus);

				window.draw(*TextCarsSpawnRateXIndicator);
				window.draw(*TextCarsSpawnRateX);
				window.draw(*ButtonCarsSpawnRateXMinus);
				window.draw(*TextButtonCarsSpawnRateXMinus);

				window.draw(*ButtonCarsSpawnRateXPlus);
				window.draw(*TextButtonCarsSpawnRateXPlus);

				window.draw(*TextTrafficLightsMode);
				window.draw(*ButtonTrafficLightsModeStatic);
				window.draw(*TextButtonTrafficLightsModeStatic);

				window.draw(*ButtonTrafficLightsModeDynamic);
				window.draw(*TextButtonTrafficLightsModeDynamic);

				window.draw(*TextWalkersSpawnRateIndicator);
				window.draw(*TextWalkersSpawnRate);
				window.draw(*ButtonWalkersSpawnRateMinus);
				window.draw(*TextButtonWalkersSpawnRateMinus);

				window.draw(*ButtonWalkersSpawnRatePlus);
				window.draw(*TextButtonWalkersSpawnRatePlus);

				window.draw(*TextTrafficLightsTimingIndicator);
				window.draw(*TextTrafficLightsTiming);
				window.draw(*ButtonTrafficLightsTimingUp);
				window.draw(*TextButtonTrafficLightsTimingUp);

				window.draw(*ButtonTrafficLightsTimingDown);
				window.draw(*TextButtonTrafficLightsTimingDown);

				window.display();
			}
			break;
		}
		case 1: {
			Event event;
			while (window.pollEvent(event))
			{
				if (event.type == Event::Closed)
					window.close();
			}

			float CurrentSeconds = RealTime.getElapsedTime().asSeconds();
			string StringCurrentTime = to_string(CurrentSeconds);

			float InModelTime = RealTime.getElapsedTime().asSeconds() * 10;
			string StringInModelTime = to_string(InModelTime);

			Text* TextRealTimeOutput = new Text(StringCurrentTime, font, 27);
			TextRealTimeOutput->setPosition(1070.0, 160.0);
			TextRealTimeOutput->setFillColor(Color(240, 240, 240));

			Text* TextInModelTime = new Text(StringInModelTime, font, 27);
			TextInModelTime->setPosition(1070.0, 300.0);
			TextInModelTime->setFillColor(Color(240, 240, 240));


			//Моделирование работы светофора
			float SwitсhTimerSeconds = SwitchTimer.getElapsedTime().asSeconds();
			//Статический режим
			if (TrafficLights[0]->TrafficlightMode == "Static") {
				for (int i = 0; i < TrafficLights.size(); i++) {
					if (TrafficLights[i]->SwitchSignalStatic(TrafficLights[i]->CurrentSignal, SwitсhTimerSeconds)) 
						if (i == TrafficLights.size() - 1) {
							SwitchTimer.restart();
							SwitсhTimerSeconds = 0;
						}
					
					if (SwitсhTimerSeconds >= TrafficLights[i]->SwitchDelay - 3) {
						TrafficLights[i]->YellowSignal->setFillColor(Color::Yellow);
						if (TrafficLights[i]->CurrentSignal == "Red")
							TrafficLights[i]->YellowSignal->setPosition(TrafficLights[i]->Signal->getPosition() + Vector2f(0, 23));
						if (TrafficLights[i]->CurrentSignal == "Green")
							TrafficLights[i]->YellowSignal->setPosition(TrafficLights[i]->Signal->getPosition() - Vector2f(0, 23));
					}
				}
			}

			//Динамический режим
			else if (SwitсhTimerSeconds > TrafficLights[0]->DynamicModeSwitchDelay) {
					for (int i = 1; i < TrafficLights.size(); i++) {
						TrafficLights[i]->DynamicModeSwitchDelay = TrafficLights[0]->DynamicModeSwitchDelay;
						if (TrafficLights[i]->SwitchSignalDynamic(TrafficLights[i]->CurrentSignal)) 
							if (i == TrafficLights.size() - 1) {
								SwitchTimer.restart();
								SwitсhTimerSeconds = 0;
							}
						if (SwitсhTimerSeconds >= TrafficLights[i]->DynamicModeSwitchDelay - 3) {
							TrafficLights[i]->YellowSignal->setFillColor(Color::Yellow);
							if (TrafficLights[i]->CurrentSignal == "Red")
								TrafficLights[i]->YellowSignal->setPosition(TrafficLights[i]->Signal->getPosition() + Vector2f(0, 23));
							if (TrafficLights[i]->CurrentSignal == "Green")
								TrafficLights[i]->YellowSignal->setPosition(TrafficLights[i]->Signal->getPosition() - Vector2f(0, 23));
						}
					}
			}

			int SpawnPositionrand;

			////Пешеходы
			float WalkersTimer = WalkersSpawnTimer.getElapsedTime().asSeconds();

			if (WalkersTimer >= 60 / WalkersSpawnRate) {
				SpawnPositionrand = rand() % 20 + 1;
				if (SpawnPositionrand < 5)
					Walkers.push_back(new Walker(0.5, 0, 515, "DownToUp", "123", "Texture/Walker.png"));
				else if (SpawnPositionrand < 10)
					Walkers.push_back(new Walker(0.5, 850, 0, "UpToDown", "123", "Texture/Walker2.png"));
				else if (SpawnPositionrand < 15)
					Walkers.push_back(new Walker(0.5, 290, 0, "LeftToRight", "123", "Texture/Walker.png"));
				else if (SpawnPositionrand < 20)
					Walkers.push_back(new Walker(0.5, 915, 690, "RightToLeft", "LeftToRight", "Texture/Walker2.png"));

				WalkersSpawnTimer.restart();
				WalkersTimer = 0;
			}

			for (int i = 0; i < Walkers.size(); i++) {/*
				if (Walkers[i]->CheckDistance(Walkers))
					break;*/
				if (Walkers[i]->Direction == "DownToUp")
					if ((Walkers[i]->WalkerTexture.getPosition().x < 280 && TrafficLights[1]->CurrentSignal == "Red") || TrafficLights[1]->CurrentSignal == "Green" || Walkers[i]->WalkerTexture.getPosition().x > 310) {
						Walkers[i]->Moving(Walkers[i]->CurrentWalkerSpeed, Walkers[i]->CurrentWalkerSpeed * -0.6);
						if (Walkers[i]->WalkerTexture.getPosition().x > 850)
							Walkers.erase(Walkers.begin() + i);
						TrafficLights[1]->WalkersCounter(Walkers[i]);
					}

				if (Walkers[i]->Direction == "UpToDown")
					if ((Walkers[i]->WalkerTexture.getPosition().x > 560 && TrafficLights[1]->CurrentSignal == "Red") || TrafficLights[1]->CurrentSignal == "Green" || Walkers[i]->WalkerTexture.getPosition().x < 530) {
						Walkers[i]->Moving(-1 * Walkers[i]->CurrentWalkerSpeed, Walkers[i]->CurrentWalkerSpeed * 0.6);
						if (Walkers[i]->WalkerTexture.getPosition().x < 10)
							Walkers.erase(Walkers.begin() + i);
						TrafficLights[1]->WalkersCounter(Walkers[i]);
					}

				if (Walkers[i]->Direction == "LeftToRight")
					if ((Walkers[i]->WalkerTexture.getPosition().x < 540 && TrafficLights[0]->CurrentSignal == "Red") || TrafficLights[0]->CurrentSignal == "Green" || Walkers[i]->WalkerTexture.getPosition().x > 570) {
						Walkers[i]->Moving(Walkers[i]->CurrentWalkerSpeed, Walkers[i]->CurrentWalkerSpeed * 0.6);
						if (Walkers[i]->WalkerTexture.getPosition().x > 1000)
							Walkers.erase(Walkers.begin() + i);
						TrafficLights[0]->WalkersCounter(Walkers[i]);
					}
				if (Walkers[i]->Direction == "RightToLeft")
					if ((Walkers[i]->WalkerTexture.getPosition().x > 600 && TrafficLights[0]->CurrentSignal == "Red") || TrafficLights[0]->CurrentSignal == "Green" || Walkers[i]->WalkerTexture.getPosition().x < 540) {
						Walkers[i]->Moving(-1 * Walkers[i]->CurrentWalkerSpeed, Walkers[i]->CurrentWalkerSpeed * -0.6);
						if (Walkers[i]->WalkerTexture.getPosition().x < 40)
							Walkers.erase(Walkers.begin() + i);
						TrafficLights[0]->WalkersCounter(Walkers[i]);
					}
			}

			//Световор для вертикальной дороги
			//Текстуры машин
			//Моделирование движения машин
			float TimerX = SpawnTimerCarX.getElapsedTime().asSeconds();
			float TimerY = SpawnTimerCarY.getElapsedTime().asSeconds();

			int SpawnRaterandX = rand() % CarsSpawnRateX + 1;
			int SpawnRaterandY = rand() % CarsSpawnRateY + 1;

			//Спавн по X
			//Скорость делится на 100, чтобы удовлетворять условия моделирования
			if (TimerX >= SpawnRaterandX) {
				SpawnPositionrand = 1;
				if (SpawnPositionrand < 5 && CheckIfSpawnClear("HorizontalLeft", CheckSpawnAreas, Cars))
					Cars.push_back(new PassengerCar(1500, 0, 60, (rand() % (SpeedUpperLimit - SpeedLowerLimit + 1) + SpeedLowerLimit) * 0.01, "Легковая", "HorizontalLeft", "Texture/CarLeftToRight.png"));
				else if (SpawnPositionrand < 11 && CheckIfSpawnClear("HorizontalRight", CheckSpawnAreas, Cars))
					Cars.push_back(new PassengerCar(1500, 900, 540, (rand() % (SpeedUpperLimit - SpeedLowerLimit + 1) + SpeedLowerLimit) * 0.01, "Легковая", "HorizontalRight", "Texture/CarRightToLeft.png"));
				Cars[Cars.size() - 1]->ChangeWay();
				SpawnTimerCarX.restart();
				TimerX = 0;

			}

			//Спавн по Y
			if (TimerY >= SpawnRaterandY) {
				SpawnPositionrand = 1;
				if (SpawnPositionrand < 5 && CheckIfSpawnClear("VerticalDown", CheckSpawnAreas, Cars)) {
					Cars.push_back(new PassengerCar(1500, 980, 0, (rand() % (SpeedUpperLimit - SpeedLowerLimit + 1) + SpeedLowerLimit) * 0.01, "Легковая", "VerticalDown", "Texture/CarUpToDown.png"));
					Cars[Cars.size() - 1]->CarTexture.setRotation(-5);
				}
				else if (SpawnPositionrand < 11 && CheckIfSpawnClear("VerticalUp", CheckSpawnAreas, Cars))
					Cars.push_back(new PassengerCar(1500, -65, 700, (rand() % (SpeedUpperLimit - SpeedLowerLimit + 1) + SpeedLowerLimit) * 0.01, "Легковая", "VerticalUp", "Texture/CarDownToUp.png"));
				Cars[Cars.size() - 1]->ChangeWay();
				SpawnTimerCarY.restart();
				TimerY = 0;
			}

			for (int i = 0; i < Cars.size(); i++) { //Поворот (Настроено)
				if (Cars[i]->Direction == "HorizontalLeft") {
					if (Cars[i]->NextDirection == "VerticalUp" && CheckRotateAreas[2]->getGlobalBounds().intersects(Cars[i]->CarTexture.getGlobalBounds())) { //Настроено
						Cars[i]->Direction = Cars[i]->NextDirection;
						Cars[i]->SetTexture("Texture/CarDownToUp.png");
					}
					else if (Cars[i]->NextDirection == "VerticalDown" && CheckRotateAreas[3]->getGlobalBounds().intersects(Cars[i]->CarTexture.getGlobalBounds())) { //Настроено
						Cars[i]->Direction = Cars[i]->NextDirection;
						Cars[i]->SetTexture("Texture/CarUpToDown.png");
					}
				}
				if (Cars[i]->Direction == "HorizontalRight") {
					if (Cars[i]->NextDirection == "VerticalUp" && CheckRotateAreas[1]->getGlobalBounds().intersects(Cars[i]->CarTexture.getGlobalBounds())) { //Настроено
						Cars[i]->Direction = Cars[i]->NextDirection;
						Cars[i]->SetTexture("Texture/CarDownToUp.png");
					}
					else if (Cars[i]->NextDirection == "VerticalDown" && CheckRotateAreas[0]->getGlobalBounds().contains(Cars[i]->CarTexture.getPosition())) { //Настроено
						Cars[i]->Direction = Cars[i]->NextDirection;
						Cars[i]->SetTexture("Texture/CarUpToDown.png");
					}
				}
				if (Cars[i]->Direction == "VerticalUp") {
					if (Cars[i]->NextDirection == "HorizontalLeft" && CheckRotateAreas[2]->getGlobalBounds().intersects(Cars[i]->CarTexture.getGlobalBounds())) { //Настроено
						Cars[i]->Direction = Cars[i]->NextDirection;
						Cars[i]->SetTexture("Texture/CarLeftToRight.png");

					}
					else if (Cars[i]->NextDirection == "HorizontalRight" && CheckRotateAreas[1]->getGlobalBounds().intersects(Cars[i]->CarTexture.getGlobalBounds())) { //Настроено
						Cars[i]->Direction = Cars[i]->NextDirection;
						Cars[i]->SetTexture("Texture/CarRightToLeft.png");
					}
				}
				if (Cars[i]->Direction == "VerticalDown") {
					if (Cars[i]->NextDirection == "HorizontalLeft" && CheckRotateAreas[3]->getGlobalBounds().intersects(Cars[i]->CarTexture.getGlobalBounds())) { //Настроено
						Cars[i]->Direction = Cars[i]->NextDirection;
						Cars[i]->SetTexture("Texture/CarLeftToRight.png");
					}
					else if (Cars[i]->NextDirection == "HorizontalRight" && CheckRotateAreas[0]->getGlobalBounds().intersects(Cars[i]->CarTexture.getGlobalBounds())) { //Настроено
						Cars[i]->Direction = Cars[i]->NextDirection;
						Cars[i]->SetTexture("Texture/CarRightToLeft.png");
					}
				}
			}

			//Движение
			for (int i = 0; i < Cars.size(); i++) {

				Cars[i]->CheckAheadCarSpeed(Cars, TrafficLights);

				Cars[i]->CheckWalker(Walkers);

				/*	if (Cars[i]->CarCrash(Cars)) {
						Cars.erase(Cars.begin() + i);
						i--;
						CarCrashesCount++;
						break;
					}*/

				if (Cars[i]->Direction == "HorizontalLeft") {
					if (CheckTrafficlightsAreas[3]->getGlobalBounds().contains(Cars[i]->CarTexture.getPosition()) && TrafficLights[0]->CurrentSignal == "Red") {
						Cars[i]->Braking();
						Cars[i]->Moving(Cars[i]->CurrentSpeed, Cars[i]->CurrentSpeed * 0.6);
					}
					else {
						Cars[i]->Acceleration();
						Cars[i]->Moving(Cars[i]->CurrentSpeed, Cars[i]->CurrentSpeed * 0.6);
					}
					if (Cars[i]->CarTexture.getPosition().x >= 1010) {
						Cars.erase(Cars.begin() + i);
						i--;
						break;
					}
					if (!Cars[i]->LineChanged && Cars[i]->NextDirection == "VerticalDown") { //Настраивается
						Cars[i]->Moving(0, 0.5);
						if (CheckLineChangeAreas[3]->getGlobalBounds().contains(Cars[i]->CarTexture.getPosition())) {
							Cars[i]->LineChanged = true;
						}
					}
					TrafficLights[0]->CarsCounter(Cars[i]);

				}
				if (Cars[i]->Direction == "HorizontalRight") {
					if (CheckTrafficlightsAreas[1]->getGlobalBounds().contains(Cars[i]->CarTexture.getPosition()) && TrafficLights[0]->CurrentSignal == "Red") {
						Cars[i]->Braking();
						Cars[i]->Moving(-1 * Cars[i]->CurrentSpeed, Cars[i]->CurrentSpeed * -0.6);
					}
					else {
						Cars[i]->Acceleration();
						Cars[i]->Moving(-1 * Cars[i]->CurrentSpeed, Cars[i]->CurrentSpeed * -0.6);
					}
					if (Cars[i]->CarTexture.getPosition().x <= 15) {
						Cars.erase(Cars.begin() + i);
						i--;
						break;
					}
					if (!Cars[i]->LineChanged && Cars[i]->NextDirection == "VerticalUp") { //Настраивается
						Cars[i]->Moving(0, -0.35);
						if (CheckLineChangeAreas[1]->getGlobalBounds().intersects(Cars[i]->CarTexture.getGlobalBounds()))
							Cars[i]->LineChanged = true;
					}
					TrafficLights[0]->CarsCounter(Cars[i]);
				}
				if (Cars[i]->Direction == "VerticalUp") {
					if (CheckTrafficlightsAreas[0]->getGlobalBounds().contains(Cars[i]->CarTexture.getPosition()) && TrafficLights[1]->CurrentSignal == "Red") {
						Cars[i]->Braking();
						Cars[i]->Moving(Cars[i]->CurrentSpeed, Cars[i]->CurrentSpeed * -0.6);
					}
					else {
						Cars[i]->Acceleration();
						Cars[i]->Moving(Cars[i]->CurrentSpeed, -0.6 * Cars[i]->CurrentSpeed);
					}
					if (Cars[i]->CarTexture.getPosition().y < 10) {
						Cars.erase(Cars.begin() + i);
						i--;
						break;
					}
					if (!Cars[i]->LineChanged && Cars[i]->NextDirection == "HorizontalLeft") { //Настраивается
						Cars[i]->Moving(0.35, 0);
						if (CheckLineChangeAreas[0]->getGlobalBounds().intersects(Cars[i]->CarTexture.getGlobalBounds())) {
							Cars[i]->LineChanged = true;
							cerr << "Kek";
						}
					}
					TrafficLights[1]->CarsCounter(Cars[i]);
				}
				if (Cars[i]->Direction == "VerticalDown") {
					if (CheckTrafficlightsAreas[2]->getGlobalBounds().contains(Cars[i]->CarTexture.getPosition()) && TrafficLights[1]->CurrentSignal == "Red") {
						Cars[i]->Braking();
						Cars[i]->Moving(-1 * Cars[i]->CurrentSpeed, Cars[i]->CurrentSpeed * 0.6);
					}
					else {
						Cars[i]->Acceleration();
						Cars[i]->Moving(-1 * Cars[i]->CurrentSpeed, 0.6 * Cars[i]->CurrentSpeed);
					}
					if (Cars[i]->CarTexture.getPosition().y >= 620) {
						Cars.erase(Cars.begin() + i);
						i--;
						break;
					}
					if (!Cars[i]->LineChanged && Cars[i]->NextDirection == "HorizontalRight" && CheckLineChangeAreas[2]->getGlobalBounds().intersects(Cars[i]->CarTexture.getGlobalBounds())) { //Настраивается
						Cars[i]->Moving(-0.35, 0);
						if (CheckLineChangeAreas[2]->getGlobalBounds().intersects(Cars[i]->CarTexture.getGlobalBounds())) {
							Cars[i]->LineChanged = true;
						}
					}
					TrafficLights[1]->CarsCounter(Cars[i]);
				}
			}

			window.clear();

			window.draw(ModelBack);
			window.draw(*CheckLineChangeAreas[0]);

			for (int i = 0; i < TrafficLights.size(); i++) {
				window.draw(*TrafficLights[i]->Signal);
				window.draw(*TrafficLights[i]->YellowSignal);
			}

			for (int i = 0; i < Cars.size(); i++) {
				window.draw(Cars[i]->CarTexture);
				//window.draw(*Cars[i]->CheckArea);
			}

			for (int i = 0; i < Walkers.size(); i++)
				window.draw(Walkers[i]->WalkerTexture);


			window.draw(ModelStat);
			window.draw(*TextRealTimeOutput);
			window.draw(*TextInModelTime);

			window.display();

		}
				break;
		}
	}
	return 0;
}