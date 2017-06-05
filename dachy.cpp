#include <iostream>
#include <vector>
#include <algorithm>

struct Krawedz;
class Dach;

class Dach {
private:
	float a;
	float b;

public:
	Dach(int x1, int y1, int x2, int y2) {
		this->a = (y1 - y2) / (x1 - x2);
		this->b = ((float)y1) - (((float)a) *x1);
	}

	int id;
	Krawedz* kA;
	Krawedz* kB;
	std::vector<Dach*> opadajace;
	int opad = 0;
	float fX(int x) { 
		//std::cout << " a " << a << " b " << b << " x " << x << "\n";
		return (float)a*x + (float)b; }

};
struct Krawedz {
	int x;
	int y;
	bool isFirst;
	Dach* dach;
};
//
//void sortEdges(std::vector<Krawedz> &krawedzie) {
//	krawedzie.insert()
//}

bool sortingFunction(Krawedz* i, Krawedz* j) { 
	if (i->x == j->x) {
		return i->y > j->y;
	}
	else
	{
		return (i->x < j->x);
	}
}



int main() {
	int roofsAmount;
	std::cin >> roofsAmount;

	std::vector<Krawedz*> krawedzie;
	std::vector<Dach*> dachy;
	Dach* highestRoof;
	int lastBroomPosition = 0;

	//Wczytywanie danych
	for (int i = 0; i < roofsAmount; i++) {
		int x1, y1, x2, y2;

		std::cin >> x1 >> y1 >> x2 >> y2;
		Krawedz* first = new Krawedz;
		first->isFirst = true;
		first->x = x1;
		first->y = y1;

		Krawedz*second = new Krawedz;
		second->isFirst = false;
		second->x = x2;
		second->y = y2;
		
		Dach* dach = new Dach(x1, y1, x2, y2);
		dach->id = i;
		dach->kA = first;
		dach->kB = second;

		first->dach = dach;
		second->dach = dach;

		krawedzie.push_back(first);
		krawedzie.push_back(second);
		dachy.push_back(dach);
	}

	//for (Dach* dach : dachy) {
	//	std::cout << "pierwsza: x " << dach->kA->x << " y " << dach->kA->y << " druga x " << dach->kB->x << " y " << dach->kB->y << "\n";
	//}

	////sortEdges(krawedzie);
	//std::cout << "unsorted:\n\n";
	//for (Krawedz* krawedz : krawedzie) {
	//	std::cout << "x " << krawedz->x << " y " << krawedz->y << " isFirst " << krawedz->isFirst << "\n";
	//}

	std::sort(krawedzie.begin(), krawedzie.end(), sortingFunction);

	//std::cout << "sorted:\n\n";
	//for (Krawedz* krawedz : krawedzie) {
	//	std::cout << "x " << krawedz->x << " y " << krawedz->y << " isFirst " << krawedz->isFirst << "\n";
	//}

	//============================================================================================

	lastBroomPosition = krawedzie[0]->x;
	highestRoof = krawedzie[0]->dach;
	int currentEdge = 0;
	std::vector<Dach*> activeRoofs;

	while (currentEdge < krawedzie.size()) {

		int currentStoppedX = krawedzie[currentEdge]->x;

		//std::cout << " lastBroomPos = " << lastBroomPosition << "\nx stopped at: " << currentStoppedX /*<< "\n"*/;

		if (highestRoof != NULL) {
			highestRoof->opad += (currentStoppedX - lastBroomPosition);
		}

		if (krawedzie[currentEdge]->isFirst) {//Je¿eli krawêdŸ siê zaczyna
			//std::cout << " first \n";
			activeRoofs.push_back(krawedzie[currentEdge]->dach);	//Dodajemy j¹ do zliczanych

			if (highestRoof == NULL || krawedzie[currentEdge]->dach->fX(currentStoppedX) > highestRoof->fX(currentStoppedX)) {		//tak: zmieniamy najwy¿szy dach
				highestRoof = krawedzie[currentEdge]->dach;
			}

			//OPADANIE KRAWÊDZI ROSN¥CEJ
			if (krawedzie[currentEdge]->dach->kA->y < krawedzie[currentEdge]->dach->kB->y) {
				//std::cout << "krawedz dachu " << krawedzie[currentEdge]->dach->id <<" jest opadajaca na ";
				int tHighestY = 0;
				Dach* tHighest = NULL;
				for (int a = 0; a < dachy.size(); a++) {
					if ((dachy[a]->kA->x <= currentStoppedX && dachy[a]->kB->x >= currentStoppedX) &&	//Je¿eli dach mieœci siê w zakresie
						dachy[a]->fX(currentStoppedX) > tHighestY && dachy[a] != highestRoof && dachy[a] != krawedzie[currentEdge]->dach) {
						tHighest = dachy[a];
						tHighestY = dachy[a]->fX(currentStoppedX);
					}
				}
				if (tHighest != NULL) {
					tHighest->opadajace.push_back(krawedzie[currentEdge]->dach);
					//std::cout << tHighest->id << "\n";
				}
				else
				{
					//std::cout << "nic \n";
				}

			}

		}
		else { //Je¿eli krawêdŸ siê koñczy
			//std::cout << " second \n";
			for (int r = 0; r < activeRoofs.size(); r++) {		//	usuwany j¹ ze zliczanych
					if (activeRoofs[r]->kB == krawedzie[currentEdge]) {
						activeRoofs.erase(activeRoofs.begin() + r);
					}
				}

			//OPADANIE KRAWÊDZI MALEJ¥CEJ
			if (krawedzie[currentEdge]->dach->kA->y > krawedzie[currentEdge]->dach->kB->y) {
				//std::cout << "krawedz dachu " << krawedzie[currentEdge]->dach->id << " jest opadajaca na ";
				int tHighestY = 0;
				Dach* tHighest = NULL;
				for (int a = 0; a < dachy.size(); a++) {
					//std::cout << " miesci siê w zakresie " << (dachy[a]->kA->x <= currentStoppedX && dachy[a]->kB->x >= currentStoppedX);
					//std::cout << " dachy[a]->fX(currentStoppedX) > tHighestY " << dachy[a]->fX(currentStoppedX) << " > " << tHighestY
					//	<< " dachy[a] id = " << dachy[a]->id << " highest id = " << highestRoof->id << " dachy[a] id = " << dachy[a]->id << " krawedzie[currentEdge]->dach id " << krawedzie[currentEdge]->dach->id << " \n";
					if ( (dachy[a]->kA->x <= currentStoppedX && dachy[a]->kB->x >= currentStoppedX) &&	//Je¿eli mieœci siê w zakresie
						dachy[a]->fX(currentStoppedX) > tHighestY && dachy[a] != highestRoof && dachy[a] != krawedzie[currentEdge]->dach) {
						tHighest = dachy[a];
						tHighestY = dachy[a]->fX(currentStoppedX);
					}
				}
				if (tHighest != NULL) {
					tHighest->opadajace.push_back(krawedzie[currentEdge]->dach);
					//std::cout << tHighest->id << "\n";
				}
				else
				{
					//std::cout << "nic \n";
				}

			}

			//Sprawdzamy ponownie najwy¿szy dach
			highestRoof = NULL;
			for (int r = 0; r < activeRoofs.size(); r++) {		
				if (highestRoof == NULL || activeRoofs[r]->fX(currentStoppedX) > highestRoof->fX(currentStoppedX)) {
					highestRoof = activeRoofs[r];
				}
			}


		}

		//for (int r = 0; r < activeRoofs.size(); r++) {		//	koñczymy zliczanie jej opadu
		//		std::cout << "aktywny dach " << activeRoofs[r]->id << "\n";
		//}
		if (highestRoof != NULL) {
			//std::cout << "highest " << highestRoof->id << "\n";
		}
		else
		{
			//std::cout << "highest is NULL";
		}

		lastBroomPosition = krawedzie[currentEdge]->x;
		currentEdge++;

		//std::cout << "\n";

		//std::cout << "x " << krawedzie[currentEdge]->x << " y " << krawedzie[currentEdge]->y << " isFirst " << krawedzie[currentEdge]->isFirst << "\n";

		//if (currentEdge != 0) {
		//	//sumujemy delta x najwy¿szej krawêdzi (zebrany opad)
		//	int newX = krawedzie[currentEdge]->x;
		//	highestRoof->opad += (newX - lastBroomPosition);

		//	//- je¿eli jest krawêdzi¹ A:
		//	if (krawedzie[currentEdge]->isFirst) {
		//		//		dodajemy jej dach do aktywnych
		//		activeRoofs.push_back(krawedzie[currentEdge]->dach);
		//		//		sprawdzamy, czy jest wy¿sza od obecnego, najwy¿szego dachu.
		//		if (krawedzie[currentEdge]->dach->fX(newX) > highestRoof->fX(newX)) {		//tak: zmieniamy najwy¿szy dach
		//			highestRoof = krawedzie[currentEdge]->dach;
		//		}

		//		//		je¿eli krawêdŸ B tego dachu jest wy¿ej ni¿ krawêdŸ A i jest pod ni¹ jakaœ krawêdŸ, dodajemy jej ten dach jako dach opadaj¹cy
		//		if (krawedzie[currentEdge]->dach->kA->y < krawedzie[currentEdge]->dach->kB->y) {
		//			int tHighestY = 0;
		//			Dach* tHighest = NULL;
		//			for (int a = 0; a < activeRoofs.size(); a++) {
		//				if (activeRoofs[a]->fX(newX) > tHighestY && activeRoofs[a] != highestRoof) {
		//					tHighest = activeRoofs[a];
		//				}
		//			}
		//			if (tHighest != NULL) {
		//				tHighest->opadajace.push_back(krawedzie[currentEdge]->dach);
		//			}
		//		}
		//	}
		//	else //- je¿eli jest krawêdzi¹ B:
		//	{
		//		for (int r = 0; r < activeRoofs.size(); r++) {		//	koñczymy zliczanie jej opadu
		//			if (activeRoofs[r]->kB == krawedzie[currentEdge]) {
		//				activeRoofs.erase(activeRoofs.begin() + r);
		//			}
		//		}

		//		if (krawedzie[currentEdge]->dach->kA->y > krawedzie[currentEdge]->dach->kB->y) {
		//			int tHighestY = 0;
		//			Dach* tHighest = NULL;
		//			for (int a = 0; a < activeRoofs.size(); a++) {
		//				if (activeRoofs[a]->fX(newX) > tHighestY && activeRoofs[a] != highestRoof) {
		//					tHighest = activeRoofs[a];
		//				}
		//			}
		//			if (tHighest != NULL) {
		//				tHighest->opadajace.push_back(krawedzie[currentEdge]->dach);
		//			}
		//		}


		//	}
		//}
		//
		//lastBroomPosition = krawedzie[currentEdge]->x;
		//currentEdge++;
	}

	for (Dach* dach : dachy) {
		/*std::cout << "dach id: " << dach->id << "pierwsza: x " << dach->kA->x << " y " << dach->kA->y << " druga x " << dach->kB->x << " y " << dach->kB->y
			<< " opad: " << dach->opad << " ";*/
		for (int o = 0; o < dach->opadajace.size(); o++) {
			//std::cout << dach->opadajace[o]->id << " ";
			dach->opad += dach->opadajace[o]->opad;
		}

		//std::cout << "dach id: " << dach->id << "pierwsza: x " << dach->kA->x << " y " << dach->kA->y << " druga x " << dach->kB->x << " y " << dach->kB->y
		//	<< " opad: " << dach->opad << " ";

		//for (int o = 0; o < dach->opadajace.size(); o++) {
		//	std::cout << dach->opadajace[o]->id << " ";
		//	//dach->opad += dach->opadajace[o]->opad;
		//}
		std::cout << dach->opad;
		std::cout << "\n";
	}

}