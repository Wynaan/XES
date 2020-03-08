#ifndef SAVEMANAGER_H_
#define SAVEMANAGER_H_

#include "interfaces.h"
#include "gui.h"
#include "xav_data.h"

class SaveManager : public IInteract, public IUpdate{
	UserInterface * GUI = nullptr;
	char slotName[4][9];
	const char slotTitle[4][7] = {
		{"Slot 1"},
		{"Slot 2"},
		{"Slot 3"},
		{"Slot 4"}
	};
	bool exit;
	bool cardDetected;
	bool validInput;
	uint8_t selection;
public:
	save_cell_t memoryBuffer;
	void OnUserInteract();
	void Tick();

	~SaveManager(){}
};




#endif /* SAVEMANAGER_H_ */
