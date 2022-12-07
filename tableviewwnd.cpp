/**********************************************************************************************************************/
//Artilab
//File: tableviewwnd.cpp
/**********************************************************************************************************************/

//Include files
#include "artilab.h"

//Static variable definitions
size_t TableViewWindow::MATRIX_NAME_SIZE = 49;
std::string TableViewWindow::MATRIX_NAME[49] = {
	"Sensory Input",
	"Sensory Input Sum",
	"Sensory Input Sum Decay",
	"Sensory Weight",
	"Sensory Weighted Input Sum",
	"Synaptic Input",
	"Synaptic Input Frequency",
	"Synaptic Input Time",
	"Synaptic Input Frequency Decay",
	"Synaptic Filter Highpass Peak",
	"Synaptic Filter Lowpass Peak",
	"Synaptic Filter Sensitivity",
	"Synaptic Filter Input",
	"Synaptic Backpropagated Output",
	"Synaptic Backpropagated Output Time",
	"Synaptic Input Sum",
	"Synaptic Input Sum Decay",
	"Synaptic Occurrence",
	"Synaptic Permanence",
	"Synaptic Weight",
	"Synaptic Weighted Input Sum",
	"Synaptic Learning Pre-spike Window",
	"Synaptic Learning Post-spike Window",
	"Synaptic Learning",
	"Modulatory Input",
	"Modulatory Base Input",
	"Modulatory Input Sum",
	"Modulatory Input Sum Decay",
	"Modulatory Weight",
	"Modulatory Weighted Input Sum",
	"Neurode Input",
	"Neurode Weight",
	"Neurode Weighted Input",
	"Neurode Backpropagated Output",
	"Neurode Weighted Backpropageted Output",
	"Oscillator Amplitude",
	"Oscillator Frequency",
	"Oscillator Phase",
	"Oscillator Base Output",
	"Oscillator Output",
	"Neurode Input Sum",
	"Neurode Input Sum Decay",
	"Neurode Activation Highpass Peak",
	"Neurode Activation Lowpass Peak",
	"Neurode Activation Sensitivity",
	"Neurode Activation",
	"Neurode Threshold",
	"Neurode Refractory Period",
	"Neurode Output"
};

//TableViewWindow constructor function
TableViewWindow::TableViewWindow(Arti* a) :
	arti(a),
	tableFlags(ImGuiTableFlags_Borders | ImGuiTableFlags_Hideable | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_NoSavedSettings),
	title(""),
	columns(0),
	rows(0),
	header(std::vector<std::string>()),
	mview(std::vector<mtxview>()),
	column(std::vector<std::vector<std::string>>())
{

}

//TableViewWindow destructor function
TableViewWindow::~TableViewWindow() {
	arti = NULL;
	tableFlags = 0;
	title = "";
	columns = 0;
	rows = 0;
	header.clear();
	mview.clear();
	column.clear();
}

//TableViewWindow open function
void TableViewWindow::open(MainViewport* mainViewport) {
	//Set window flags
	wndFlags = NoCollapse | Closable | NoResize | NoMove;

	//Position and size the window
	if (anchor || resizeWnd) {
		wndSize = { mainViewport->workSize().width, mainViewport->workSize().height };
		wndPos = { mainViewport->workPosition().x, mainViewport->workPosition().y };
		ImGui::SetNextWindowPos(ImVec2(wndPos.x, wndPos.y));
		ImGui::SetNextWindowSize(ImVec2(wndSize.width, wndSize.height));
		anchor = false;
		resizeWnd = false;
	}

	//Create window
	if (create("Neural Table View", wndFlags)) {
		//Set number of rows
		rows = ((int)(wndSize.height - ImGui::GetTextLineHeightWithSpacing() * 3) / (int)ImGui::GetTextLineHeightWithSpacing()) - 1;

		//Table data
		static std::vector<std::vector<float>> tableData;

		//Create customize region
		ImGui::BeginChild("Customize Region", ImVec2(wndSize.width / 5.0f, 0.0f), false, NULL);

		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (ImGui::GetContentRegionAvail().x / 2) - (ImGui::CalcTextSize("Customize View").x / 2));
		ImGui::Text("Customize View");

		//Create title input text
		ImGui::Text("Title: ");

		static std::string titleBuf = "New Table";
		title = titleBuf;

		ImGui::SameLine();
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
		if (ImGui::InputText("##Title", titleBuf.data(), 256)) {
			title = titleBuf.c_str();
		}

		if (title.empty()) {
			titleBuf = "New Table";
			title = titleBuf;
		}

		//Create columns input text
		ImGui::Text("Columns: ");

		size_t prevColSize = columns;

		ImGui::SameLine();
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
		ImGui::SliderInt("##Columns", &columns, 0, 64);

		//If columns exist
		if (columns > 0) {
			//Create column headers input text list
			ImGui::Separator();
			ImGui::Text("Column Headers");

			static std::vector<std::string> headerBuf = std::vector<std::string>();
			headerBuf.resize(columns);
			header.resize(columns);

			for (int i = 0; i < header.size(); ++i) {
				if (header[i].empty()) {
					header[i] = std::format("Column {}", i + 1);
					headerBuf[i] = header[i];
				}
			}

			if (ImGui::BeginListBox("##HeaderList", 
				ImVec2(-FLT_MIN, min((26.0f * columns) + 2.0f, ImGui::GetContentRegionAvail().y / 3)))) {
				for (int i = 0; i < columns; ++i) {
					ImGui::Text("Column %2d:", i + 1);

					ImGui::SameLine();
					ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
					if (ImGui::InputText(std::format("##Column{}", i).c_str(), headerBuf[i].data(), 256)) {
						header[i] = headerBuf[i].c_str();
					}
				}

				ImGui::EndListBox();
			}

			//Create column matrix selection inputs
			ImGui::Separator();
			ImGui::Text("Column Matrix Selection");

			static int selectedCol = 1;
			static std::string colName;
			static std::vector<int> selectedNrlMtx;
			static std::vector<int> selectedMtx;
			static std::vector<std::vector<int>> selectedRow;
			static std::vector<std::vector<int>> selectedNrd;
			selectedNrlMtx.resize(columns);
			selectedMtx.resize(columns);
			selectedRow.resize(columns);
			selectedNrd.resize(columns);
			tableData.resize(columns);

			for (int i = (int)prevColSize; i < columns; ++i) {
				selectedRow[i] = std::vector<int>(MATRIX_NAME_SIZE);
				selectedNrd[i] = std::vector<int>(MATRIX_NAME_SIZE);
				tableData[i] = std::vector<float>(rows);
			}

			//Create column selection combobox
			ImGui::Text("Column: ");

			ImGui::SameLine();
			ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
			if (ImGui::BeginCombo("##ColumnCombo", header[selectedCol - 1].c_str(),
				ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_HeightLarge)) {
				for (int i = 0; i < header.size(); ++i) {
					const bool is_selected = (selectedCol - 1 == i);

					if (ImGui::Selectable(header[i].c_str(), is_selected)) {
						selectedCol = i + 1;
					}

					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}

				ImGui::EndCombo();
			}

			//Create neural matrix selection combobox
			static std::vector<std::string> nmName = std::vector<std::string>(arti->neuralMatrices());
			for (int i = 0; i < (int)arti->neuralMatrices(); ++i)
				nmName[i] = arti->getNmName(i);

			ImGui::Text("Neural Matrix: ");

			ImGui::SameLine();
			ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
			if (nmName.size() > 0 && ImGui::BeginCombo(std::format("##NeuralMatrixCombo{}", selectedCol - 1).c_str(), 
				nmName[selectedNrlMtx[selectedCol - 1]].c_str(), ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_HeightLarge)) {
				for (int i = 0; i < nmName.size(); ++i) {
					const bool is_selected = (selectedNrlMtx[selectedCol - 1] == i);

					if (ImGui::Selectable(nmName[i].c_str(), is_selected))
						selectedNrlMtx[selectedCol - 1] = i;

					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}

				ImGui::EndCombo();
			}

			//Create matrix selection combobox
			ImGui::Text("Matrix: ");

			ImGui::SameLine();
			ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
			if (ImGui::BeginCombo(std::format("##MatrixCombo{}", selectedCol - 1).c_str(), MATRIX_NAME[selectedMtx[selectedCol - 1]].c_str(),
				ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_HeightLarge)) {
				for (int i = 0; i < MATRIX_NAME_SIZE; ++i) {
					const bool is_selected = (selectedMtx[selectedCol - 1] == i);

					if (ImGui::Selectable(MATRIX_NAME[i].c_str(), is_selected))
						selectedMtx[selectedCol - 1] = i;

					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}

				ImGui::EndCombo();
			}

			static std::vector<std::string> senName = std::vector<std::string>(arti->sensoryInputs(nmName[selectedNrlMtx[selectedCol - 1]]));
			static std::vector<std::string> nrdName = std::vector<std::string>(arti->neurodes(nmName[selectedNrlMtx[selectedCol - 1]]));

			for (int i = 0; i < (int)arti->sensoryInputs(nmName[selectedNrlMtx[selectedCol - 1]]); ++i)
				senName[i] = arti->getSenName(i, nmName[selectedNrlMtx[selectedCol - 1]]);

			for (int i = 0; i < (int)arti->neurodes(nmName[selectedNrlMtx[selectedCol - 1]]); ++i)
				nrdName[i] = arti->getNrdName(i, nmName[selectedNrlMtx[selectedCol - 1]]);

			switch (selectedMtx[selectedCol - 1]) {
			case SENSORY_INPUT:
				//Create sensory input selection combobox
				ImGui::Text("Sensory Row: ");

				ImGui::SameLine();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo(std::format("##SensoryInputCombo{}", selectedCol - 1).c_str(), 
					senName[selectedRow[selectedCol - 1][SENSORY_INPUT]].c_str(),
					ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_HeightLarge)) {
					for (int i = 0; i < senName.size(); ++i) {
						const bool is_selected = (selectedRow[selectedCol - 1][SENSORY_INPUT] == i);

						if (ImGui::Selectable(senName[i].c_str(), is_selected))
							selectedRow[selectedCol - 1][SENSORY_INPUT] = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				//Set table data at selected column
				for (int i = rows - 1; i > 0; --i)
					tableData[selectedCol - 1][i] = tableData[selectedCol - 1][i - 1];

				tableData[selectedCol - 1][0] = arti->neuralMatrix(nmName[selectedNrlMtx[selectedCol - 1]])->
					getSenXView().row(selectedRow[selectedCol - 1][SENSORY_INPUT]).at(0);
				break;
			case SENSORY_INPUT_SUM:
				//Create sensory input selection combobox
				ImGui::Text("Sensory Row: ");

				ImGui::SameLine();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo(std::format("##SensoryInputCombo{}", selectedCol - 1).c_str(), 
					senName[selectedRow[selectedCol - 1][SENSORY_INPUT_SUM]].c_str(),
					ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_HeightLarge)) {
					for (int i = 0; i < senName.size(); ++i) {
						const bool is_selected = (selectedRow[selectedCol - 1][SENSORY_INPUT_SUM] == i);

						if (ImGui::Selectable(senName[i].c_str(), is_selected))
							selectedRow[selectedCol - 1][SENSORY_INPUT_SUM] = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				//Create neurode selection combobox
				ImGui::Text("Neurode Column: ");

				ImGui::SameLine();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo(std::format("##SensoryInputNeurodeCombo{}", selectedCol - 1).c_str(),
					nrdName[selectedNrd[selectedCol - 1][SENSORY_INPUT_SUM]].c_str(),
					ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_HeightLarge)) {
					for (int i = 0; i < nrdName.size(); ++i) {
						const bool is_selected = (selectedNrd[selectedCol - 1][SENSORY_INPUT_SUM] == i);

						if (ImGui::Selectable(nrdName[i].c_str(), is_selected))
							selectedNrd[selectedCol - 1][SENSORY_INPUT_SUM] = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				//Set table data at selected column
				for (int i = rows - 1; i > 0; --i)
					tableData[selectedCol - 1][i] = tableData[selectedCol - 1][i - 1];

				tableData[selectedCol - 1][0] = arti->neuralMatrix(nmName[selectedNrlMtx[selectedCol - 1]])->
					getSenSView().row(selectedRow[selectedCol - 1][SENSORY_INPUT_SUM]).at(selectedNrd[selectedCol - 1][SENSORY_INPUT_SUM]);
				break;
			case SENSORY_INPUT_SUM_DECAY:
				//Create sensory input selection combobox
				ImGui::Text("Sensory Row: ");

				ImGui::SameLine();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo(std::format("##SensoryInputCombo{}", selectedCol - 1).c_str(), 
					senName[selectedRow[selectedCol - 1][SENSORY_INPUT_SUM_DECAY]].c_str(),
					ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_HeightLarge)) {
					for (int i = 0; i < senName.size(); ++i) {
						const bool is_selected = (selectedRow[selectedCol - 1][SENSORY_INPUT_SUM_DECAY] == i);

						if (ImGui::Selectable(senName[i].c_str(), is_selected))
							selectedRow[selectedCol - 1][SENSORY_INPUT_SUM_DECAY] = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				//Create neurode selection combobox
				ImGui::Text("Neurode Column: ");

				ImGui::SameLine();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo(std::format("##SensoryInputNeurodeCombo{}", selectedCol - 1).c_str(),
					nrdName[selectedNrd[selectedCol - 1][SENSORY_INPUT_SUM_DECAY]].c_str(),
					ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_HeightLarge)) {
					for (int i = 0; i < nrdName.size(); ++i) {
						const bool is_selected = (selectedNrd[selectedCol - 1][SENSORY_INPUT_SUM_DECAY] == i);

						if (ImGui::Selectable(nrdName[i].c_str(), is_selected))
							selectedNrd[selectedCol - 1][SENSORY_INPUT_SUM_DECAY] = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				//Set table data at selected column
				for (int i = rows - 1; i > 0; --i)
					tableData[selectedCol - 1][i] = tableData[selectedCol - 1][i - 1];

				tableData[selectedCol - 1][0] = arti->neuralMatrix(nmName[selectedNrlMtx[selectedCol - 1]])->
					getSenSdView().row(selectedRow[selectedCol - 1][SENSORY_INPUT_SUM_DECAY]).at(selectedNrd[selectedCol - 1][SENSORY_INPUT_SUM_DECAY]);
				break;
			case SENSORY_WEIGHT:
				//Create sensory input selection combobox
				ImGui::Text("Sensory Row: ");

				ImGui::SameLine();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo(std::format("##SensoryInputRowCombo{}", selectedCol - 1).c_str(), 
					senName[selectedRow[selectedCol - 1][SENSORY_WEIGHT]].c_str(),
					ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_HeightLarge)) {
					for (int i = 0; i < senName.size(); ++i) {
						const bool is_selected = (selectedRow[selectedCol - 1][SENSORY_WEIGHT] == i);

						if (ImGui::Selectable(senName[i].c_str(), is_selected))
							selectedRow[selectedCol - 1][SENSORY_WEIGHT] = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				//Create neurode selection combobox
				ImGui::Text("Neurode Column: ");

				ImGui::SameLine();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo(std::format("##SensoryInputNeurodeCombo{}", selectedCol - 1).c_str(),
					nrdName[selectedNrd[selectedCol - 1][SENSORY_WEIGHT]].c_str(),
					ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_HeightLarge)) {
					for (int i = 0; i < nrdName.size(); ++i) {
						const bool is_selected = (selectedNrd[selectedCol - 1][SENSORY_WEIGHT] == i);

						if (ImGui::Selectable(nrdName[i].c_str(), is_selected))
							selectedNrd[selectedCol - 1][SENSORY_WEIGHT] = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				//Set table data at selected column
				for (int i = rows - 1; i > 0; --i)
					tableData[selectedCol - 1][i] = tableData[selectedCol - 1][i - 1];

				tableData[selectedCol - 1][0] = arti->neuralMatrix(nmName[selectedNrlMtx[selectedCol - 1]])->
					getSenWView().row(selectedRow[selectedCol - 1][SENSORY_WEIGHT]).at(selectedNrd[selectedCol - 1][SENSORY_WEIGHT]);
				break;
			case SENSORY_WEIGHTED_INPUT_SUM:
				//Create sensory input selection combobox
				ImGui::Text("Sensory Row: ");

				ImGui::SameLine();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo(std::format("##SensoryInputRowCombo{}", selectedCol - 1).c_str(), 
					senName[selectedRow[selectedCol - 1][SENSORY_WEIGHTED_INPUT_SUM]].c_str(),
					ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_HeightLarge)) {
					for (int i = 0; i < senName.size(); ++i) {
						const bool is_selected = (selectedRow[selectedCol - 1][SENSORY_WEIGHTED_INPUT_SUM] == i);

						if (ImGui::Selectable(senName[i].c_str(), is_selected))
							selectedRow[selectedCol - 1][SENSORY_WEIGHTED_INPUT_SUM] = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				//Create neurode selection combobox
				ImGui::Text("Neurode Column: ");

				ImGui::SameLine();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo(std::format("##SensoryInputNeurodeCombo{}", selectedCol - 1).c_str(),
					nrdName[selectedNrd[selectedCol - 1][SENSORY_WEIGHTED_INPUT_SUM]].c_str(),
					ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_HeightLarge)) {
					for (int i = 0; i < nrdName.size(); ++i) {
						const bool is_selected = (selectedNrd[selectedCol - 1][SENSORY_WEIGHTED_INPUT_SUM] == i);

						if (ImGui::Selectable(nrdName[i].c_str(), is_selected))
							selectedNrd[selectedCol - 1][SENSORY_WEIGHTED_INPUT_SUM] = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				//Set table data at selected column
				for (int i = rows - 1; i > 0; --i)
					tableData[selectedCol - 1][i] = tableData[selectedCol - 1][i - 1];

				tableData[selectedCol - 1][0] = arti->neuralMatrix(nmName[selectedNrlMtx[selectedCol - 1]])->
					getSenSwView().row(selectedRow[selectedCol - 1][SENSORY_WEIGHTED_INPUT_SUM]).at(selectedNrd[selectedCol - 1][SENSORY_WEIGHTED_INPUT_SUM]);
				break;
			case SYNAPTIC_INPUT:
				//Create neurode selection combobox
				ImGui::Text("Neurode Row: ");

				ImGui::SameLine();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo(std::format("##SynapticInputRowCombo{}", selectedCol - 1).c_str(), 
					nrdName[selectedRow[selectedCol - 1][SYNAPTIC_INPUT]].c_str(),
					ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_HeightLarge)) {
					for (int i = 0; i < nrdName.size(); ++i) {
						const bool is_selected = (selectedRow[selectedCol - 1][SYNAPTIC_INPUT] == i);

						if (ImGui::Selectable(nrdName[i].c_str(), is_selected))
							selectedRow[selectedCol - 1][SYNAPTIC_INPUT] = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				//Create neurode selection combobox
				ImGui::Text("Neurode Column: ");

				ImGui::SameLine();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo(std::format("##SynapticInputNeurodeCombo{}", selectedCol - 1).c_str(),
					nrdName[selectedNrd[selectedCol - 1][SYNAPTIC_INPUT]].c_str(),
					ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_HeightLarge)) {
					for (int i = 0; i < nrdName.size(); ++i) {
						const bool is_selected = (selectedNrd[selectedCol - 1][SYNAPTIC_INPUT] == i);

						if (ImGui::Selectable(nrdName[i].c_str(), is_selected))
							selectedNrd[selectedCol - 1][SYNAPTIC_INPUT] = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				//Set table data at selected column
				for (int i = rows - 1; i > 0; --i)
					tableData[selectedCol - 1][i] = tableData[selectedCol - 1][i - 1];

				tableData[selectedCol - 1][0] = arti->neuralMatrix(nmName[selectedNrlMtx[selectedCol - 1]])->
					getSynXView().row(selectedRow[selectedCol - 1][SYNAPTIC_INPUT]).at(selectedNrd[selectedCol - 1][SYNAPTIC_INPUT]);
				break;
			case SYNAPTIC_INPUT_FREQUENCY:
				//Create neurode selection combobox
				ImGui::Text("Neurode Row: ");

				ImGui::SameLine();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo(std::format("##SynapticInputRowCombo{}", selectedCol - 1).c_str(),
					nrdName[selectedRow[selectedCol - 1][SYNAPTIC_INPUT_FREQUENCY]].c_str(),
					ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_HeightLarge)) {
					for (int i = 0; i < nrdName.size(); ++i) {
						const bool is_selected = (selectedRow[selectedCol - 1][SYNAPTIC_INPUT_FREQUENCY] == i);

						if (ImGui::Selectable(nrdName[i].c_str(), is_selected))
							selectedRow[selectedCol - 1][SYNAPTIC_INPUT_FREQUENCY] = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				//Create neurode selection combobox
				ImGui::Text("Neurode Column: ");

				ImGui::SameLine();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo(std::format("##SynapticInputNeurodeCombo{}", selectedCol - 1).c_str(),
					nrdName[selectedNrd[selectedCol - 1][SYNAPTIC_INPUT_FREQUENCY]].c_str(),
					ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_HeightLarge)) {
					for (int i = 0; i < nrdName.size(); ++i) {
						const bool is_selected = (selectedNrd[selectedCol - 1][SYNAPTIC_INPUT_FREQUENCY] == i);

						if (ImGui::Selectable(nrdName[i].c_str(), is_selected))
							selectedNrd[selectedCol - 1][SYNAPTIC_INPUT_FREQUENCY] = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				//Set table data at selected column
				for (int i = rows - 1; i > 0; --i)
					tableData[selectedCol - 1][i] = tableData[selectedCol - 1][i - 1];

				tableData[selectedCol - 1][0] = arti->neuralMatrix(nmName[selectedNrlMtx[selectedCol - 1]])->
					getSynXfView().row(selectedRow[selectedCol - 1][SYNAPTIC_INPUT_FREQUENCY]).at(selectedNrd[selectedCol - 1][SYNAPTIC_INPUT_FREQUENCY]);
				break;
			case SYNAPTIC_INPUT_TIME:
				//Create neurode selection combobox
				ImGui::Text("Neurode Row: ");

				ImGui::SameLine();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo(std::format("##SynapticInputRowCombo{}", selectedCol - 1).c_str(),
					nrdName[selectedRow[selectedCol - 1][SYNAPTIC_INPUT_TIME]].c_str(),
					ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_HeightLarge)) {
					for (int i = 0; i < nrdName.size(); ++i) {
						const bool is_selected = (selectedRow[selectedCol - 1][SYNAPTIC_INPUT_TIME] == i);

						if (ImGui::Selectable(nrdName[i].c_str(), is_selected))
							selectedRow[selectedCol - 1][SYNAPTIC_INPUT_TIME] = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				//Create neurode selection combobox
				ImGui::Text("Neurode Column: ");

				ImGui::SameLine();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo(std::format("##SynapticInputNeurodeCombo{}", selectedCol - 1).c_str(),
					nrdName[selectedNrd[selectedCol - 1][SYNAPTIC_INPUT_TIME]].c_str(),
					ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_HeightLarge)) {
					for (int i = 0; i < nrdName.size(); ++i) {
						const bool is_selected = (selectedNrd[selectedCol - 1][SYNAPTIC_INPUT_TIME] == i);

						if (ImGui::Selectable(nrdName[i].c_str(), is_selected))
							selectedNrd[selectedCol - 1][SYNAPTIC_INPUT_TIME] = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				//Set table data at selected column
				for (int i = rows - 1; i > 0; --i)
					tableData[selectedCol - 1][i] = tableData[selectedCol - 1][i - 1];

				tableData[selectedCol - 1][0] = arti->neuralMatrix(nmName[selectedNrlMtx[selectedCol - 1]])->
					getSynXtView().row(selectedRow[selectedCol - 1][SYNAPTIC_INPUT_TIME]).at(selectedNrd[selectedCol - 1][SYNAPTIC_INPUT_TIME]);
				break;
			case SYNAPTIC_INPUT_FREQUENCY_DECAY:
				//Create neurode selection combobox
				ImGui::Text("Neurode Row: ");

				ImGui::SameLine();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo(std::format("##SynapticInputRowCombo{}", selectedCol - 1).c_str(),
					nrdName[selectedRow[selectedCol - 1][SYNAPTIC_INPUT_FREQUENCY_DECAY]].c_str(),
					ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_HeightLarge)) {
					for (int i = 0; i < nrdName.size(); ++i) {
						const bool is_selected = (selectedRow[selectedCol - 1][SYNAPTIC_INPUT_FREQUENCY_DECAY] == i);

						if (ImGui::Selectable(nrdName[i].c_str(), is_selected))
							selectedRow[selectedCol - 1][SYNAPTIC_INPUT_FREQUENCY_DECAY] = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				//Create neurode selection combobox
				ImGui::Text("Neurode Column: ");

				ImGui::SameLine();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo(std::format("##SynapticInputNeurodeCombo{}", selectedCol - 1).c_str(),
					nrdName[selectedNrd[selectedCol - 1][SYNAPTIC_INPUT_FREQUENCY_DECAY]].c_str(),
					ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_HeightLarge)) {
					for (int i = 0; i < nrdName.size(); ++i) {
						const bool is_selected = (selectedNrd[selectedCol - 1][SYNAPTIC_INPUT_FREQUENCY_DECAY] == i);

						if (ImGui::Selectable(nrdName[i].c_str(), is_selected))
							selectedNrd[selectedCol - 1][SYNAPTIC_INPUT_FREQUENCY_DECAY] = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				//Set table data at selected column
				for (int i = rows - 1; i > 0; --i)
					tableData[selectedCol - 1][i] = tableData[selectedCol - 1][i - 1];

				tableData[selectedCol - 1][0] = arti->neuralMatrix(nmName[selectedNrlMtx[selectedCol - 1]])->
					getSynXfdView().row(selectedRow[selectedCol - 1][SYNAPTIC_INPUT_FREQUENCY_DECAY]).at(selectedNrd[selectedCol - 1][SYNAPTIC_INPUT_FREQUENCY_DECAY]);
				break;
			case SYNAPTIC_FILTER_HIGHPASS_PEAK:
				//Create neurode selection combobox
				ImGui::Text("Neurode Row: ");

				ImGui::SameLine();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo(std::format("##SynapticInputRowCombo{}", selectedCol - 1).c_str(),
					nrdName[selectedRow[selectedCol - 1][SYNAPTIC_FILTER_HIGHPASS_PEAK]].c_str(),
					ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_HeightLarge)) {
					for (int i = 0; i < nrdName.size(); ++i) {
						const bool is_selected = (selectedRow[selectedCol - 1][SYNAPTIC_FILTER_HIGHPASS_PEAK] == i);

						if (ImGui::Selectable(nrdName[i].c_str(), is_selected))
							selectedRow[selectedCol - 1][SYNAPTIC_FILTER_HIGHPASS_PEAK] = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				//Create neurode selection combobox
				ImGui::Text("Neurode Column: ");

				ImGui::SameLine();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo(std::format("##SynapticInputNeurodeCombo{}", selectedCol - 1).c_str(),
					nrdName[selectedNrd[selectedCol - 1][SYNAPTIC_FILTER_HIGHPASS_PEAK]].c_str(),
					ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_HeightLarge)) {
					for (int i = 0; i < nrdName.size(); ++i) {
						const bool is_selected = (selectedNrd[selectedCol - 1][SYNAPTIC_FILTER_HIGHPASS_PEAK] == i);

						if (ImGui::Selectable(nrdName[i].c_str(), is_selected))
							selectedNrd[selectedCol - 1][SYNAPTIC_FILTER_HIGHPASS_PEAK] = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				//Set table data at selected column
				for (int i = rows - 1; i > 0; --i)
					tableData[selectedCol - 1][i] = tableData[selectedCol - 1][i - 1];

				tableData[selectedCol - 1][0] = arti->neuralMatrix(nmName[selectedNrlMtx[selectedCol - 1]])->
					getSynFhView().row(selectedRow[selectedCol - 1][SYNAPTIC_FILTER_HIGHPASS_PEAK]).at(selectedNrd[selectedCol - 1][SYNAPTIC_FILTER_HIGHPASS_PEAK]);
				break;
			case SYNAPTIC_FILTER_LOWPASS_PEAK:
				//Create neurode selection combobox
				ImGui::Text("Neurode Row: ");

				ImGui::SameLine();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo(std::format("##SynapticInputRowCombo{}", selectedCol - 1).c_str(),
					nrdName[selectedRow[selectedCol - 1][SYNAPTIC_FILTER_LOWPASS_PEAK]].c_str(),
					ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_HeightLarge)) {
					for (int i = 0; i < nrdName.size(); ++i) {
						const bool is_selected = (selectedRow[selectedCol - 1][SYNAPTIC_FILTER_LOWPASS_PEAK] == i);

						if (ImGui::Selectable(nrdName[i].c_str(), is_selected))
							selectedRow[selectedCol - 1][SYNAPTIC_FILTER_LOWPASS_PEAK] = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				//Create neurode selection combobox
				ImGui::Text("Neurode Column: ");

				ImGui::SameLine();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo(std::format("##SynapticInputNeurodeCombo{}", selectedCol - 1).c_str(),
					nrdName[selectedNrd[selectedCol - 1][SYNAPTIC_FILTER_LOWPASS_PEAK]].c_str(),
					ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_HeightLarge)) {
					for (int i = 0; i < nrdName.size(); ++i) {
						const bool is_selected = (selectedNrd[selectedCol - 1][SYNAPTIC_FILTER_LOWPASS_PEAK] == i);

						if (ImGui::Selectable(nrdName[i].c_str(), is_selected))
							selectedNrd[selectedCol - 1][SYNAPTIC_FILTER_LOWPASS_PEAK] = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				//Set table data at selected column
				for (int i = rows - 1; i > 0; --i)
					tableData[selectedCol - 1][i] = tableData[selectedCol - 1][i - 1];

				tableData[selectedCol - 1][0] = arti->neuralMatrix(nmName[selectedNrlMtx[selectedCol - 1]])->
					getSynFlView().row(selectedRow[selectedCol - 1][SYNAPTIC_FILTER_LOWPASS_PEAK]).at(selectedNrd[selectedCol - 1][SYNAPTIC_FILTER_LOWPASS_PEAK]);
				break;
			case SYNAPTIC_FILTER_SENSITIVITY:
				//Create neurode selection combobox
				ImGui::Text("Neurode Row: ");

				ImGui::SameLine();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo(std::format("##SynapticInputRowCombo{}", selectedCol - 1).c_str(),
					nrdName[selectedRow[selectedCol - 1][SYNAPTIC_FILTER_SENSITIVITY]].c_str(),
					ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_HeightLarge)) {
					for (int i = 0; i < nrdName.size(); ++i) {
						const bool is_selected = (selectedRow[selectedCol - 1][SYNAPTIC_FILTER_SENSITIVITY] == i);

						if (ImGui::Selectable(nrdName[i].c_str(), is_selected))
							selectedRow[selectedCol - 1][SYNAPTIC_FILTER_SENSITIVITY] = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				//Create neurode selection combobox
				ImGui::Text("Neurode Column: ");

				ImGui::SameLine();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo(std::format("##SynapticInputNeurodeCombo{}", selectedCol - 1).c_str(),
					nrdName[selectedNrd[selectedCol - 1][SYNAPTIC_FILTER_SENSITIVITY]].c_str(),
					ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_HeightLarge)) {
					for (int i = 0; i < nrdName.size(); ++i) {
						const bool is_selected = (selectedNrd[selectedCol - 1][SYNAPTIC_FILTER_SENSITIVITY] == i);

						if (ImGui::Selectable(nrdName[i].c_str(), is_selected))
							selectedNrd[selectedCol - 1][SYNAPTIC_FILTER_SENSITIVITY] = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				//Set table data at selected column
				for (int i = rows - 1; i > 0; --i)
					tableData[selectedCol - 1][i] = tableData[selectedCol - 1][i - 1];

				tableData[selectedCol - 1][0] = arti->neuralMatrix(nmName[selectedNrlMtx[selectedCol - 1]])->
					getSynFsView().row(selectedRow[selectedCol - 1][SYNAPTIC_FILTER_SENSITIVITY]).at(selectedNrd[selectedCol - 1][SYNAPTIC_FILTER_SENSITIVITY]);
				break;
			case SYNAPTIC_FILTER_INPUT:
				//Create neurode selection combobox
				ImGui::Text("Neurode Row: ");

				ImGui::SameLine();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo(std::format("##SynapticInputRowCombo{}", selectedCol - 1).c_str(),
					nrdName[selectedRow[selectedCol - 1][SYNAPTIC_FILTER_INPUT]].c_str(),
					ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_HeightLarge)) {
					for (int i = 0; i < nrdName.size(); ++i) {
						const bool is_selected = (selectedRow[selectedCol - 1][SYNAPTIC_FILTER_INPUT] == i);

						if (ImGui::Selectable(nrdName[i].c_str(), is_selected))
							selectedRow[selectedCol - 1][SYNAPTIC_FILTER_INPUT] = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				//Create neurode selection combobox
				ImGui::Text("Neurode Column: ");

				ImGui::SameLine();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo(std::format("##SynapticInputNeurodeCombo{}", selectedCol - 1).c_str(),
					nrdName[selectedNrd[selectedCol - 1][SYNAPTIC_FILTER_INPUT]].c_str(),
					ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_HeightLarge)) {
					for (int i = 0; i < nrdName.size(); ++i) {
						const bool is_selected = (selectedNrd[selectedCol - 1][SYNAPTIC_FILTER_INPUT] == i);

						if (ImGui::Selectable(nrdName[i].c_str(), is_selected))
							selectedNrd[selectedCol - 1][SYNAPTIC_FILTER_INPUT] = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				//Set table data at selected column
				for (int i = rows - 1; i > 0; --i)
					tableData[selectedCol - 1][i] = tableData[selectedCol - 1][i - 1];

				tableData[selectedCol - 1][0] = arti->neuralMatrix(nmName[selectedNrlMtx[selectedCol - 1]])->
					getSynFView().row(selectedRow[selectedCol - 1][SYNAPTIC_FILTER_INPUT]).at(selectedNrd[selectedCol - 1][SYNAPTIC_FILTER_INPUT]);
				break;
			case SYNAPTIC_BACKPROPAGATED_OUTPUT:
				//Create neurode selection combobox
				ImGui::Text("Neurode Row: ");

				ImGui::SameLine();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo(std::format("##SynapticInputRowCombo{}", selectedCol - 1).c_str(),
					nrdName[selectedRow[selectedCol - 1][SYNAPTIC_BACKPROPAGATED_OUTPUT]].c_str(),
					ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_HeightLarge)) {
					for (int i = 0; i < nrdName.size(); ++i) {
						const bool is_selected = (selectedRow[selectedCol - 1][SYNAPTIC_BACKPROPAGATED_OUTPUT] == i);

						if (ImGui::Selectable(nrdName[i].c_str(), is_selected))
							selectedRow[selectedCol - 1][SYNAPTIC_BACKPROPAGATED_OUTPUT] = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				//Create neurode selection combobox
				ImGui::Text("Neurode Column: ");

				ImGui::SameLine();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo(std::format("##SynapticInputNeurodeCombo{}", selectedCol - 1).c_str(),
					nrdName[selectedNrd[selectedCol - 1][SYNAPTIC_BACKPROPAGATED_OUTPUT]].c_str(),
					ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_HeightLarge)) {
					for (int i = 0; i < nrdName.size(); ++i) {
						const bool is_selected = (selectedNrd[selectedCol - 1][SYNAPTIC_BACKPROPAGATED_OUTPUT] == i);

						if (ImGui::Selectable(nrdName[i].c_str(), is_selected))
							selectedNrd[selectedCol - 1][SYNAPTIC_BACKPROPAGATED_OUTPUT] = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				//Set table data at selected column
				for (int i = rows - 1; i > 0; --i)
					tableData[selectedCol - 1][i] = tableData[selectedCol - 1][i - 1];

				tableData[selectedCol - 1][0] = arti->neuralMatrix(nmName[selectedNrlMtx[selectedCol - 1]])->
					getSynYpView().row(selectedRow[selectedCol - 1][SYNAPTIC_BACKPROPAGATED_OUTPUT]).at(selectedNrd[selectedCol - 1][SYNAPTIC_BACKPROPAGATED_OUTPUT]);
				break;
			case SYNAPTIC_BACKPROPAGATED_OUTPUT_TIME:
				//Create neurode selection combobox
				ImGui::Text("Neurode Row: ");

				ImGui::SameLine();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo(std::format("##SynapticInputRowCombo{}", selectedCol - 1).c_str(),
					nrdName[selectedRow[selectedCol - 1][SYNAPTIC_BACKPROPAGATED_OUTPUT_TIME]].c_str(),
					ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_HeightLarge)) {
					for (int i = 0; i < nrdName.size(); ++i) {
						const bool is_selected = (selectedRow[selectedCol - 1][SYNAPTIC_BACKPROPAGATED_OUTPUT_TIME] == i);

						if (ImGui::Selectable(nrdName[i].c_str(), is_selected))
							selectedRow[selectedCol - 1][SYNAPTIC_BACKPROPAGATED_OUTPUT_TIME] = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				//Create neurode selection combobox
				ImGui::Text("Neurode Column: ");

				ImGui::SameLine();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo(std::format("##SynapticInputNeurodeCombo{}", selectedCol - 1).c_str(),
					nrdName[selectedNrd[selectedCol - 1][SYNAPTIC_BACKPROPAGATED_OUTPUT_TIME]].c_str(),
					ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_HeightLarge)) {
					for (int i = 0; i < nrdName.size(); ++i) {
						const bool is_selected = (selectedNrd[selectedCol - 1][SYNAPTIC_BACKPROPAGATED_OUTPUT_TIME] == i);

						if (ImGui::Selectable(nrdName[i].c_str(), is_selected))
							selectedNrd[selectedCol - 1][SYNAPTIC_BACKPROPAGATED_OUTPUT_TIME] = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				//Set table data at selected column
				for (int i = rows - 1; i > 0; --i)
					tableData[selectedCol - 1][i] = tableData[selectedCol - 1][i - 1];

				tableData[selectedCol - 1][0] = arti->neuralMatrix(nmName[selectedNrlMtx[selectedCol - 1]])->
					getSynYptView().row(selectedRow[selectedCol - 1][SYNAPTIC_BACKPROPAGATED_OUTPUT_TIME]).
					at(selectedNrd[selectedCol - 1][SYNAPTIC_BACKPROPAGATED_OUTPUT_TIME]);
				break;
			case SYNAPTIC_INPUT_SUM:
				//Create neurode selection combobox
				ImGui::Text("Neurode Row: ");

				ImGui::SameLine();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo(std::format("##SynapticInputRowCombo{}", selectedCol - 1).c_str(),
					nrdName[selectedRow[selectedCol - 1][SYNAPTIC_INPUT_SUM]].c_str(),
					ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_HeightLarge)) {
					for (int i = 0; i < nrdName.size(); ++i) {
						const bool is_selected = (selectedRow[selectedCol - 1][SYNAPTIC_INPUT_SUM] == i);

						if (ImGui::Selectable(nrdName[i].c_str(), is_selected))
							selectedRow[selectedCol - 1][SYNAPTIC_INPUT_SUM] = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				//Create neurode selection combobox
				ImGui::Text("Neurode Column: ");

				ImGui::SameLine();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo(std::format("##SynapticInputNeurodeCombo{}", selectedCol - 1).c_str(),
					nrdName[selectedNrd[selectedCol - 1][SYNAPTIC_INPUT_SUM]].c_str(),
					ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_HeightLarge)) {
					for (int i = 0; i < nrdName.size(); ++i) {
						const bool is_selected = (selectedNrd[selectedCol - 1][SYNAPTIC_INPUT_SUM] == i);

						if (ImGui::Selectable(nrdName[i].c_str(), is_selected))
							selectedNrd[selectedCol - 1][SYNAPTIC_INPUT_SUM] = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				//Set table data at selected column
				for (int i = rows - 1; i > 0; --i)
					tableData[selectedCol - 1][i] = tableData[selectedCol - 1][i - 1];

				tableData[selectedCol - 1][0] = arti->neuralMatrix(nmName[selectedNrlMtx[selectedCol - 1]])->
					getSynSView().row(selectedRow[selectedCol - 1][SYNAPTIC_INPUT_SUM]).at(selectedNrd[selectedCol - 1][SYNAPTIC_INPUT_SUM]);
				break;
			case SYNAPTIC_INPUT_SUM_DECAY:
				//Create neurode selection combobox
				ImGui::Text("Neurode Row: ");

				ImGui::SameLine();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo(std::format("##SynapticInputRowCombo{}", selectedCol - 1).c_str(),
					nrdName[selectedRow[selectedCol - 1][SYNAPTIC_INPUT_SUM_DECAY]].c_str(),
					ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_HeightLarge)) {
					for (int i = 0; i < nrdName.size(); ++i) {
						const bool is_selected = (selectedRow[selectedCol - 1][SYNAPTIC_INPUT_SUM_DECAY] == i);

						if (ImGui::Selectable(nrdName[i].c_str(), is_selected))
							selectedRow[selectedCol - 1][SYNAPTIC_INPUT_SUM_DECAY] = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				//Create neurode selection combobox
				ImGui::Text("Neurode Column: ");

				ImGui::SameLine();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo(std::format("##SynapticInputNeurodeCombo{}", selectedCol - 1).c_str(),
					nrdName[selectedNrd[selectedCol - 1][SYNAPTIC_INPUT_SUM_DECAY]].c_str(),
					ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_HeightLarge)) {
					for (int i = 0; i < nrdName.size(); ++i) {
						const bool is_selected = (selectedNrd[selectedCol - 1][SYNAPTIC_INPUT_SUM_DECAY] == i);

						if (ImGui::Selectable(nrdName[i].c_str(), is_selected))
							selectedNrd[selectedCol - 1][SYNAPTIC_INPUT_SUM_DECAY] = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				//Set table data at selected column
				for (int i = rows - 1; i > 0; --i)
					tableData[selectedCol - 1][i] = tableData[selectedCol - 1][i - 1];

				tableData[selectedCol - 1][0] = arti->neuralMatrix(nmName[selectedNrlMtx[selectedCol - 1]])->
					getSynSdView().row(selectedRow[selectedCol - 1][SYNAPTIC_INPUT_SUM_DECAY]).at(selectedNrd[selectedCol - 1][SYNAPTIC_INPUT_SUM_DECAY]);
				break;
			case SYNAPTIC_OCCURRENCE:
				//Create neurode selection combobox
				ImGui::Text("Neurode Row: ");

				ImGui::SameLine();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo(std::format("##SynapticInputRowCombo{}", selectedCol - 1).c_str(),
					nrdName[selectedRow[selectedCol - 1][SYNAPTIC_OCCURRENCE]].c_str(),
					ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_HeightLarge)) {
					for (int i = 0; i < nrdName.size(); ++i) {
						const bool is_selected = (selectedRow[selectedCol - 1][SYNAPTIC_OCCURRENCE] == i);

						if (ImGui::Selectable(nrdName[i].c_str(), is_selected))
							selectedRow[selectedCol - 1][SYNAPTIC_OCCURRENCE] = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				//Create neurode selection combobox
				ImGui::Text("Neurode Column: ");

				ImGui::SameLine();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo(std::format("##SynapticInputNeurodeCombo{}", selectedCol - 1).c_str(),
					nrdName[selectedNrd[selectedCol - 1][SYNAPTIC_OCCURRENCE]].c_str(),
					ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_HeightLarge)) {
					for (int i = 0; i < nrdName.size(); ++i) {
						const bool is_selected = (selectedNrd[selectedCol - 1][SYNAPTIC_OCCURRENCE] == i);

						if (ImGui::Selectable(nrdName[i].c_str(), is_selected))
							selectedNrd[selectedCol - 1][SYNAPTIC_OCCURRENCE] = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				//Set table data at selected column
				for (int i = rows - 1; i > 0; --i)
					tableData[selectedCol - 1][i] = tableData[selectedCol - 1][i - 1];

				tableData[selectedCol - 1][0] = arti->neuralMatrix(nmName[selectedNrlMtx[selectedCol - 1]])->
					getSynOView().row(selectedRow[selectedCol - 1][SYNAPTIC_OCCURRENCE]).at(selectedNrd[selectedCol - 1][SYNAPTIC_OCCURRENCE]);
				break;
			case SYNAPTIC_PERMANENCE:
				//Create neurode selection combobox
				ImGui::Text("Neurode Row: ");

				ImGui::SameLine();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo(std::format("##SynapticInputRowCombo{}", selectedCol - 1).c_str(),
					nrdName[selectedRow[selectedCol - 1][SYNAPTIC_PERMANENCE]].c_str(),
					ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_HeightLarge)) {
					for (int i = 0; i < nrdName.size(); ++i) {
						const bool is_selected = (selectedRow[selectedCol - 1][SYNAPTIC_PERMANENCE] == i);

						if (ImGui::Selectable(nrdName[i].c_str(), is_selected))
							selectedRow[selectedCol - 1][SYNAPTIC_PERMANENCE] = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				//Create neurode selection combobox
				ImGui::Text("Neurode Column: ");

				ImGui::SameLine();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo(std::format("##SynapticInputNeurodeCombo{}", selectedCol - 1).c_str(),
					nrdName[selectedNrd[selectedCol - 1][SYNAPTIC_PERMANENCE]].c_str(),
					ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_HeightLarge)) {
					for (int i = 0; i < nrdName.size(); ++i) {
						const bool is_selected = (selectedNrd[selectedCol - 1][SYNAPTIC_PERMANENCE] == i);

						if (ImGui::Selectable(nrdName[i].c_str(), is_selected))
							selectedNrd[selectedCol - 1][SYNAPTIC_PERMANENCE] = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				//Set table data at selected column
				for (int i = rows - 1; i > 0; --i)
					tableData[selectedCol - 1][i] = tableData[selectedCol - 1][i - 1];

				tableData[selectedCol - 1][0] = arti->neuralMatrix(nmName[selectedNrlMtx[selectedCol - 1]])->
					getSynPView().row(selectedRow[selectedCol - 1][SYNAPTIC_PERMANENCE]).at(selectedNrd[selectedCol - 1][SYNAPTIC_PERMANENCE]);
				break;
			case SYNAPTIC_WEIGHT:
				//Create neurode selection combobox
				ImGui::Text("Neurode Row: ");

				ImGui::SameLine();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo(std::format("##SynapticInputRowCombo{}", selectedCol - 1).c_str(),
					nrdName[selectedRow[selectedCol - 1][SYNAPTIC_WEIGHT]].c_str(),
					ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_HeightLarge)) {
					for (int i = 0; i < nrdName.size(); ++i) {
						const bool is_selected = (selectedRow[selectedCol - 1][SYNAPTIC_WEIGHT] == i);

						if (ImGui::Selectable(nrdName[i].c_str(), is_selected))
							selectedRow[selectedCol - 1][SYNAPTIC_WEIGHT] = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				//Create neurode selection combobox
				ImGui::Text("Neurode Column: ");

				ImGui::SameLine();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo(std::format("##SynapticInputNeurodeCombo{}", selectedCol - 1).c_str(),
					nrdName[selectedNrd[selectedCol - 1][SYNAPTIC_WEIGHT]].c_str(),
					ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_HeightLarge)) {
					for (int i = 0; i < nrdName.size(); ++i) {
						const bool is_selected = (selectedNrd[selectedCol - 1][SYNAPTIC_WEIGHT] == i);

						if (ImGui::Selectable(nrdName[i].c_str(), is_selected))
							selectedNrd[selectedCol - 1][SYNAPTIC_WEIGHT] = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				//Set table data at selected column
				for (int i = rows - 1; i > 0; --i)
					tableData[selectedCol - 1][i] = tableData[selectedCol - 1][i - 1];

				tableData[selectedCol - 1][0] = arti->neuralMatrix(nmName[selectedNrlMtx[selectedCol - 1]])->
					getSynWView().row(selectedRow[selectedCol - 1][SYNAPTIC_WEIGHT]).at(selectedNrd[selectedCol - 1][SYNAPTIC_WEIGHT]);
				break;
			case SYNAPTIC_WEIGHTED_INPUT_SUM:
				//Create neurode selection combobox
				ImGui::Text("Neurode Row: ");

				ImGui::SameLine();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo(std::format("##SynapticInputRowCombo{}", selectedCol - 1).c_str(),
					nrdName[selectedRow[selectedCol - 1][SYNAPTIC_WEIGHTED_INPUT_SUM]].c_str(),
					ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_HeightLarge)) {
					for (int i = 0; i < nrdName.size(); ++i) {
						const bool is_selected = (selectedRow[selectedCol - 1][SYNAPTIC_WEIGHTED_INPUT_SUM] == i);

						if (ImGui::Selectable(nrdName[i].c_str(), is_selected))
							selectedRow[selectedCol - 1][SYNAPTIC_WEIGHTED_INPUT_SUM] = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				//Create neurode selection combobox
				ImGui::Text("Neurode Column: ");

				ImGui::SameLine();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo(std::format("##SynapticInputNeurodeCombo{}", selectedCol - 1).c_str(),
					nrdName[selectedNrd[selectedCol - 1][SYNAPTIC_WEIGHTED_INPUT_SUM]].c_str(),
					ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_HeightLarge)) {
					for (int i = 0; i < nrdName.size(); ++i) {
						const bool is_selected = (selectedNrd[selectedCol - 1][SYNAPTIC_WEIGHTED_INPUT_SUM] == i);

						if (ImGui::Selectable(nrdName[i].c_str(), is_selected))
							selectedNrd[selectedCol - 1][SYNAPTIC_WEIGHTED_INPUT_SUM] = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				//Set table data at selected column
				for (int i = rows - 1; i > 0; --i)
					tableData[selectedCol - 1][i] = tableData[selectedCol - 1][i - 1];

				tableData[selectedCol - 1][0] = arti->neuralMatrix(nmName[selectedNrlMtx[selectedCol - 1]])->
					getSynSwView().row(selectedRow[selectedCol - 1][SYNAPTIC_WEIGHTED_INPUT_SUM]).at(selectedNrd[selectedCol - 1][SYNAPTIC_WEIGHTED_INPUT_SUM]);
				break;
			case SYNAPTIC_LEARNING_PRESPIKE_WINDOW:
				//Create neurode selection combobox
				ImGui::Text("Neurode Row: ");

				ImGui::SameLine();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo(std::format("##SynapticInputRowCombo{}", selectedCol - 1).c_str(),
					nrdName[selectedRow[selectedCol - 1][SYNAPTIC_LEARNING_PRESPIKE_WINDOW]].c_str(),
					ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_HeightLarge)) {
					for (int i = 0; i < nrdName.size(); ++i) {
						const bool is_selected = (selectedRow[selectedCol - 1][SYNAPTIC_LEARNING_PRESPIKE_WINDOW] == i);

						if (ImGui::Selectable(nrdName[i].c_str(), is_selected))
							selectedRow[selectedCol - 1][SYNAPTIC_LEARNING_PRESPIKE_WINDOW] = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				//Create neurode selection combobox
				ImGui::Text("Neurode Column: ");

				ImGui::SameLine();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo(std::format("##SynapticInputNeurodeCombo{}", selectedCol - 1).c_str(),
					nrdName[selectedNrd[selectedCol - 1][SYNAPTIC_LEARNING_PRESPIKE_WINDOW]].c_str(),
					ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_HeightLarge)) {
					for (int i = 0; i < nrdName.size(); ++i) {
						const bool is_selected = (selectedNrd[selectedCol - 1][SYNAPTIC_LEARNING_PRESPIKE_WINDOW] == i);

						if (ImGui::Selectable(nrdName[i].c_str(), is_selected))
							selectedNrd[selectedCol - 1][SYNAPTIC_LEARNING_PRESPIKE_WINDOW] = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				//Set table data at selected column
				for (int i = rows - 1; i > 0; --i)
					tableData[selectedCol - 1][i] = tableData[selectedCol - 1][i - 1];

				tableData[selectedCol - 1][0] = arti->neuralMatrix(nmName[selectedNrlMtx[selectedCol - 1]])->
					getSynLpreView().row(selectedRow[selectedCol - 1][SYNAPTIC_LEARNING_PRESPIKE_WINDOW]).
					at(selectedNrd[selectedCol - 1][SYNAPTIC_LEARNING_PRESPIKE_WINDOW]);
				break;
			case SYNAPTIC_LEARNING_POSTSPIKE_WINDOW:
				//Create neurode selection combobox
				ImGui::Text("Neurode Row: ");

				ImGui::SameLine();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo(std::format("##SynapticInputRowCombo{}", selectedCol - 1).c_str(),
					nrdName[selectedRow[selectedCol - 1][SYNAPTIC_LEARNING_POSTSPIKE_WINDOW]].c_str(),
					ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_HeightLarge)) {
					for (int i = 0; i < nrdName.size(); ++i) {
						const bool is_selected = (selectedRow[selectedCol - 1][SYNAPTIC_LEARNING_POSTSPIKE_WINDOW] == i);

						if (ImGui::Selectable(nrdName[i].c_str(), is_selected))
							selectedRow[selectedCol - 1][SYNAPTIC_LEARNING_POSTSPIKE_WINDOW] = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				//Create neurode selection combobox
				ImGui::Text("Neurode Column: ");

				ImGui::SameLine();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo(std::format("##SynapticInputNeurodeCombo{}", selectedCol - 1).c_str(),
					nrdName[selectedNrd[selectedCol - 1][SYNAPTIC_LEARNING_POSTSPIKE_WINDOW]].c_str(),
					ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_HeightLarge)) {
					for (int i = 0; i < nrdName.size(); ++i) {
						const bool is_selected = (selectedNrd[selectedCol - 1][SYNAPTIC_LEARNING_POSTSPIKE_WINDOW] == i);

						if (ImGui::Selectable(nrdName[i].c_str(), is_selected))
							selectedNrd[selectedCol - 1][SYNAPTIC_LEARNING_POSTSPIKE_WINDOW] = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				//Set table data at selected column
				for (int i = rows - 1; i > 0; --i)
					tableData[selectedCol - 1][i] = tableData[selectedCol - 1][i - 1];

				tableData[selectedCol - 1][0] = arti->neuralMatrix(nmName[selectedNrlMtx[selectedCol - 1]])->
					getSynLpostView().row(selectedRow[selectedCol - 1][SYNAPTIC_LEARNING_POSTSPIKE_WINDOW]).
					at(selectedNrd[selectedCol - 1][SYNAPTIC_LEARNING_POSTSPIKE_WINDOW]);
				break;
			case SYNAPTIC_LEARNING:
				//Create neurode selection combobox
				ImGui::Text("Neurode Row: ");

				ImGui::SameLine();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo(std::format("##SynapticInputRowCombo{}", selectedCol - 1).c_str(),
					nrdName[selectedRow[selectedCol - 1][SYNAPTIC_LEARNING]].c_str(),
					ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_HeightLarge)) {
					for (int i = 0; i < nrdName.size(); ++i) {
						const bool is_selected = (selectedRow[selectedCol - 1][SYNAPTIC_LEARNING] == i);

						if (ImGui::Selectable(nrdName[i].c_str(), is_selected))
							selectedRow[selectedCol - 1][SYNAPTIC_LEARNING] = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				//Create neurode selection combobox
				ImGui::Text("Neurode Column: ");

				ImGui::SameLine();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo(std::format("##SynapticInputNeurodeCombo{}", selectedCol - 1).c_str(),
					nrdName[selectedNrd[selectedCol - 1][SYNAPTIC_LEARNING]].c_str(),
					ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_HeightLarge)) {
					for (int i = 0; i < nrdName.size(); ++i) {
						const bool is_selected = (selectedNrd[selectedCol - 1][SYNAPTIC_LEARNING] == i);

						if (ImGui::Selectable(nrdName[i].c_str(), is_selected))
							selectedNrd[selectedCol - 1][SYNAPTIC_LEARNING] = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				//Set table data at selected column
				for (int i = rows - 1; i > 0; --i)
					tableData[selectedCol - 1][i] = tableData[selectedCol - 1][i - 1];

				tableData[selectedCol - 1][0] = arti->neuralMatrix(nmName[selectedNrlMtx[selectedCol - 1]])->
					getSynLView().row(selectedRow[selectedCol - 1][SYNAPTIC_LEARNING]).at(selectedNrd[selectedCol - 1][SYNAPTIC_LEARNING]);
				break;
			case MODULATORY_INPUT:
				//Create neurode selection combobox
				ImGui::Text("Neurode Row: ");

				ImGui::SameLine();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo(std::format("##ModulatoryInputRowCombo{}", selectedCol - 1).c_str(),
					nrdName[selectedRow[selectedCol - 1][MODULATORY_INPUT]].c_str(),
					ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_HeightLarge)) {
					for (int i = 0; i < nrdName.size(); ++i) {
						const bool is_selected = (selectedRow[selectedCol - 1][MODULATORY_INPUT] == i);

						if (ImGui::Selectable(nrdName[i].c_str(), is_selected))
							selectedRow[selectedCol - 1][MODULATORY_INPUT] = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				//Create neurode selection combobox
				ImGui::Text("Neurode Column: ");

				ImGui::SameLine();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo(std::format("##ModulatoryInputNeurodeCombo{}", selectedCol - 1).c_str(),
					nrdName[selectedNrd[selectedCol - 1][MODULATORY_INPUT]].c_str(),
					ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_HeightLarge)) {
					for (int i = 0; i < nrdName.size(); ++i) {
						const bool is_selected = (selectedNrd[selectedCol - 1][MODULATORY_INPUT] == i);

						if (ImGui::Selectable(nrdName[i].c_str(), is_selected))
							selectedNrd[selectedCol - 1][MODULATORY_INPUT] = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				//Set table data at selected column
				for (int i = rows - 1; i > 0; --i)
					tableData[selectedCol - 1][i] = tableData[selectedCol - 1][i - 1];

				tableData[selectedCol - 1][0] = arti->neuralMatrix(nmName[selectedNrlMtx[selectedCol - 1]])->
					getModXView().row(selectedRow[selectedCol - 1][MODULATORY_INPUT]).at(selectedNrd[selectedCol - 1][MODULATORY_INPUT]);
				break;
			case MODULATORY_BASE_INPUT:
				//Create neurode selection combobox
				ImGui::Text("Neurode Row: ");

				ImGui::SameLine();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo(std::format("##ModulatoryInputRowCombo{}", selectedCol - 1).c_str(),
					nrdName[selectedRow[selectedCol - 1][MODULATORY_BASE_INPUT]].c_str(),
					ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_HeightLarge)) {
					for (int i = 0; i < nrdName.size(); ++i) {
						const bool is_selected = (selectedRow[selectedCol - 1][MODULATORY_BASE_INPUT] == i);

						if (ImGui::Selectable(nrdName[i].c_str(), is_selected))
							selectedRow[selectedCol - 1][MODULATORY_BASE_INPUT] = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				//Create neurode selection combobox
				ImGui::Text("Neurode Column: ");

				ImGui::SameLine();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo(std::format("##ModulatoryInputNeurodeCombo{}", selectedCol - 1).c_str(),
					nrdName[selectedNrd[selectedCol - 1][MODULATORY_BASE_INPUT]].c_str(),
					ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_HeightLarge)) {
					for (int i = 0; i < nrdName.size(); ++i) {
						const bool is_selected = (selectedNrd[selectedCol - 1][MODULATORY_BASE_INPUT] == i);

						if (ImGui::Selectable(nrdName[i].c_str(), is_selected))
							selectedNrd[selectedCol - 1][MODULATORY_BASE_INPUT] = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				//Set table data at selected column
				for (int i = rows - 1; i > 0; --i)
					tableData[selectedCol - 1][i] = tableData[selectedCol - 1][i - 1];

				tableData[selectedCol - 1][0] = arti->neuralMatrix(nmName[selectedNrlMtx[selectedCol - 1]])->
					getModXbView().row(selectedRow[selectedCol - 1][MODULATORY_BASE_INPUT]).at(selectedNrd[selectedCol - 1][MODULATORY_BASE_INPUT]);
				break;
			case MODULATORY_INPUT_SUM:
				//Create neurode selection combobox
				ImGui::Text("Neurode Row: ");

				ImGui::SameLine();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo(std::format("##ModulatoryInputRowCombo{}", selectedCol - 1).c_str(),
					nrdName[selectedRow[selectedCol - 1][MODULATORY_INPUT_SUM]].c_str(),
					ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_HeightLarge)) {
					for (int i = 0; i < nrdName.size(); ++i) {
						const bool is_selected = (selectedRow[selectedCol - 1][MODULATORY_INPUT_SUM] == i);

						if (ImGui::Selectable(nrdName[i].c_str(), is_selected))
							selectedRow[selectedCol - 1][MODULATORY_INPUT_SUM] = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				//Create neurode selection combobox
				ImGui::Text("Neurode Column: ");

				ImGui::SameLine();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo(std::format("##ModulatoryInputNeurodeCombo{}", selectedCol - 1).c_str(),
					nrdName[selectedNrd[selectedCol - 1][MODULATORY_INPUT_SUM]].c_str(),
					ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_HeightLarge)) {
					for (int i = 0; i < nrdName.size(); ++i) {
						const bool is_selected = (selectedNrd[selectedCol - 1][MODULATORY_INPUT_SUM] == i);

						if (ImGui::Selectable(nrdName[i].c_str(), is_selected))
							selectedNrd[selectedCol - 1][MODULATORY_INPUT_SUM] = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				//Set table data at selected column
				for (int i = rows - 1; i > 0; --i)
					tableData[selectedCol - 1][i] = tableData[selectedCol - 1][i - 1];

				tableData[selectedCol - 1][0] = arti->neuralMatrix(nmName[selectedNrlMtx[selectedCol - 1]])->
					getModSView().row(selectedRow[selectedCol - 1][MODULATORY_INPUT_SUM]).at(selectedNrd[selectedCol - 1][MODULATORY_INPUT_SUM]);
				break;
			case MODULATORY_INPUT_SUM_DECAY:
				//Create neurode selection combobox
				ImGui::Text("Neurode Row: ");

				ImGui::SameLine();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo(std::format("##ModulatoryInputRowCombo{}", selectedCol - 1).c_str(),
					nrdName[selectedRow[selectedCol - 1][MODULATORY_INPUT_SUM_DECAY]].c_str(),
					ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_HeightLarge)) {
					for (int i = 0; i < nrdName.size(); ++i) {
						const bool is_selected = (selectedRow[selectedCol - 1][MODULATORY_INPUT_SUM_DECAY] == i);

						if (ImGui::Selectable(nrdName[i].c_str(), is_selected))
							selectedRow[selectedCol - 1][MODULATORY_INPUT_SUM_DECAY] = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				//Create neurode selection combobox
				ImGui::Text("Neurode Column: ");

				ImGui::SameLine();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo(std::format("##ModulatoryInputNeurodeCombo{}", selectedCol - 1).c_str(),
					nrdName[selectedNrd[selectedCol - 1][MODULATORY_INPUT_SUM_DECAY]].c_str(),
					ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_HeightLarge)) {
					for (int i = 0; i < nrdName.size(); ++i) {
						const bool is_selected = (selectedNrd[selectedCol - 1][MODULATORY_INPUT_SUM_DECAY] == i);

						if (ImGui::Selectable(nrdName[i].c_str(), is_selected))
							selectedNrd[selectedCol - 1][MODULATORY_INPUT_SUM_DECAY] = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				//Set table data at selected column
				for (int i = rows - 1; i > 0; --i)
					tableData[selectedCol - 1][i] = tableData[selectedCol - 1][i - 1];

				tableData[selectedCol - 1][0] = arti->neuralMatrix(nmName[selectedNrlMtx[selectedCol - 1]])->
					getModSdView().row(selectedRow[selectedCol - 1][MODULATORY_INPUT_SUM_DECAY]).at(selectedNrd[selectedCol - 1][MODULATORY_INPUT_SUM_DECAY]);
				break;
			case MODULATORY_WEIGHT:
				//Create neurode selection combobox
				ImGui::Text("Neurode Row: ");

				ImGui::SameLine();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo(std::format("##ModulatoryInputRowCombo{}", selectedCol - 1).c_str(),
					nrdName[selectedRow[selectedCol - 1][MODULATORY_WEIGHT]].c_str(),
					ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_HeightLarge)) {
					for (int i = 0; i < nrdName.size(); ++i) {
						const bool is_selected = (selectedRow[selectedCol - 1][MODULATORY_WEIGHT] == i);

						if (ImGui::Selectable(nrdName[i].c_str(), is_selected))
							selectedRow[selectedCol - 1][MODULATORY_WEIGHT] = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				//Create neurode selection combobox
				ImGui::Text("Neurode Column: ");

				ImGui::SameLine();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo(std::format("##ModulatoryInputNeurodeCombo{}", selectedCol - 1).c_str(),
					nrdName[selectedNrd[selectedCol - 1][MODULATORY_WEIGHT]].c_str(),
					ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_HeightLarge)) {
					for (int i = 0; i < nrdName.size(); ++i) {
						const bool is_selected = (selectedNrd[selectedCol - 1][MODULATORY_WEIGHT] == i);

						if (ImGui::Selectable(nrdName[i].c_str(), is_selected))
							selectedNrd[selectedCol - 1][MODULATORY_WEIGHT] = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				//Set table data at selected column
				for (int i = rows - 1; i > 0; --i)
					tableData[selectedCol - 1][i] = tableData[selectedCol - 1][i - 1];

				tableData[selectedCol - 1][0] = arti->neuralMatrix(nmName[selectedNrlMtx[selectedCol - 1]])->
					getModWView().row(selectedRow[selectedCol - 1][MODULATORY_WEIGHT]).at(selectedNrd[selectedCol - 1][MODULATORY_WEIGHT]);
				break;
			case MODULATORY_WEIGHTED_INPUT_SUM:
				//Create neurode selection combobox
				ImGui::Text("Neurode Row: ");

				ImGui::SameLine();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo(std::format("##ModulatoryInputRowCombo{}", selectedCol - 1).c_str(),
					nrdName[selectedRow[selectedCol - 1][MODULATORY_WEIGHTED_INPUT_SUM]].c_str(),
					ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_HeightLarge)) {
					for (int i = 0; i < nrdName.size(); ++i) {
						const bool is_selected = (selectedRow[selectedCol - 1][MODULATORY_WEIGHTED_INPUT_SUM] == i);

						if (ImGui::Selectable(nrdName[i].c_str(), is_selected))
							selectedRow[selectedCol - 1][MODULATORY_WEIGHTED_INPUT_SUM] = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				//Create neurode selection combobox
				ImGui::Text("Neurode Column: ");

				ImGui::SameLine();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo(std::format("##ModulatoryInputNeurodeCombo{}", selectedCol - 1).c_str(),
					nrdName[selectedNrd[selectedCol - 1][MODULATORY_WEIGHTED_INPUT_SUM]].c_str(),
					ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_HeightLarge)) {
					for (int i = 0; i < nrdName.size(); ++i) {
						const bool is_selected = (selectedNrd[selectedCol - 1][MODULATORY_WEIGHTED_INPUT_SUM] == i);

						if (ImGui::Selectable(nrdName[i].c_str(), is_selected))
							selectedNrd[selectedCol - 1][MODULATORY_WEIGHTED_INPUT_SUM] = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				//Set table data at selected column
				for (int i = rows - 1; i > 0; --i)
					tableData[selectedCol - 1][i] = tableData[selectedCol - 1][i - 1];

				tableData[selectedCol - 1][0] = arti->neuralMatrix(nmName[selectedNrlMtx[selectedCol - 1]])->
					getModSwView().row(selectedRow[selectedCol - 1][MODULATORY_WEIGHTED_INPUT_SUM]).at(selectedNrd[selectedCol - 1][MODULATORY_WEIGHTED_INPUT_SUM]);
				break;
			case NEURODE_INPUT:
				//Create neurode selection combobox
				ImGui::Text("Neurode Row: ");

				ImGui::SameLine();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo(std::format("##NeurodeInputRowCombo{}", selectedCol - 1).c_str(),
					nrdName[selectedRow[selectedCol - 1][NEURODE_INPUT]].c_str(),
					ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_HeightLarge)) {
					for (int i = 0; i < nrdName.size(); ++i) {
						const bool is_selected = (selectedRow[selectedCol - 1][NEURODE_INPUT] == i);

						if (ImGui::Selectable(nrdName[i].c_str(), is_selected))
							selectedRow[selectedCol - 1][NEURODE_INPUT] = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				//Create neurode selection combobox
				ImGui::Text("Neurode Column: ");

				ImGui::SameLine();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo(std::format("##NeurodeInputNeurodeCombo{}", selectedCol - 1).c_str(),
					nrdName[selectedNrd[selectedCol - 1][NEURODE_INPUT]].c_str(),
					ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_HeightLarge)) {
					for (int i = 0; i < nrdName.size(); ++i) {
						const bool is_selected = (selectedNrd[selectedCol - 1][NEURODE_INPUT] == i);

						if (ImGui::Selectable(nrdName[i].c_str(), is_selected))
							selectedNrd[selectedCol - 1][NEURODE_INPUT] = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				//Set table data at selected column
				for (int i = rows - 1; i > 0; --i)
					tableData[selectedCol - 1][i] = tableData[selectedCol - 1][i - 1];

				tableData[selectedCol - 1][0] = arti->neuralMatrix(nmName[selectedNrlMtx[selectedCol - 1]])->
					getNrdXView().row(selectedRow[selectedCol - 1][NEURODE_INPUT]).at(selectedNrd[selectedCol - 1][NEURODE_INPUT]);
				break;
			case NEURODE_WEIGHT:
				//Create neurode selection combobox
				ImGui::Text("Neurode Row: ");

				ImGui::SameLine();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo(std::format("##NeurodeInputRowCombo{}", selectedCol - 1).c_str(),
					nrdName[selectedRow[selectedCol - 1][NEURODE_WEIGHT]].c_str(),
					ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_HeightLarge)) {
					for (int i = 0; i < nrdName.size(); ++i) {
						const bool is_selected = (selectedRow[selectedCol - 1][NEURODE_WEIGHT] == i);

						if (ImGui::Selectable(nrdName[i].c_str(), is_selected))
							selectedRow[selectedCol - 1][NEURODE_WEIGHT] = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				//Create neurode selection combobox
				ImGui::Text("Neurode Column: ");

				ImGui::SameLine();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo(std::format("##NeurodeInputNeurodeCombo{}", selectedCol - 1).c_str(),
					nrdName[selectedNrd[selectedCol - 1][NEURODE_WEIGHT]].c_str(),
					ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_HeightLarge)) {
					for (int i = 0; i < nrdName.size(); ++i) {
						const bool is_selected = (selectedNrd[selectedCol - 1][NEURODE_WEIGHT] == i);

						if (ImGui::Selectable(nrdName[i].c_str(), is_selected))
							selectedNrd[selectedCol - 1][NEURODE_WEIGHT] = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				//Set table data at selected column
				for (int i = rows - 1; i > 0; --i)
					tableData[selectedCol - 1][i] = tableData[selectedCol - 1][i - 1];

				tableData[selectedCol - 1][0] = arti->neuralMatrix(nmName[selectedNrlMtx[selectedCol - 1]])->
					getNrdWView().row(selectedRow[selectedCol - 1][NEURODE_WEIGHT]).at(selectedNrd[selectedCol - 1][NEURODE_WEIGHT]);
				break;
			case NEURODE_WEIGHTED_INPUT:
				//Create neurode selection combobox
				ImGui::Text("Neurode Row: ");

				ImGui::SameLine();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo(std::format("##NeurodeInputRowCombo{}", selectedCol - 1).c_str(),
					nrdName[selectedRow[selectedCol - 1][NEURODE_WEIGHTED_INPUT]].c_str(),
					ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_HeightLarge)) {
					for (int i = 0; i < nrdName.size(); ++i) {
						const bool is_selected = (selectedRow[selectedCol - 1][NEURODE_WEIGHTED_INPUT] == i);

						if (ImGui::Selectable(nrdName[i].c_str(), is_selected))
							selectedRow[selectedCol - 1][NEURODE_WEIGHTED_INPUT] = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				//Create neurode selection combobox
				ImGui::Text("Neurode Column: ");

				ImGui::SameLine();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo(std::format("##NeurodeInputNeurodeCombo{}", selectedCol - 1).c_str(),
					nrdName[selectedNrd[selectedCol - 1][NEURODE_WEIGHTED_INPUT]].c_str(),
					ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_HeightLarge)) {
					for (int i = 0; i < nrdName.size(); ++i) {
						const bool is_selected = (selectedNrd[selectedCol - 1][NEURODE_WEIGHTED_INPUT] == i);

						if (ImGui::Selectable(nrdName[i].c_str(), is_selected))
							selectedNrd[selectedCol - 1][NEURODE_WEIGHTED_INPUT] = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				//Set table data at selected column
				for (int i = rows - 1; i > 0; --i)
					tableData[selectedCol - 1][i] = tableData[selectedCol - 1][i - 1];

				tableData[selectedCol - 1][0] = arti->neuralMatrix(nmName[selectedNrlMtx[selectedCol - 1]])->
					getNrdXwView().row(selectedRow[selectedCol - 1][NEURODE_WEIGHTED_INPUT]).at(selectedNrd[selectedCol - 1][NEURODE_WEIGHTED_INPUT]);
				break;
			case NEURODE_BACKPROPAGATED_OUTPUT:
				//Create neurode selection combobox
				ImGui::Text("Neurode Row: ");

				ImGui::SameLine();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo(std::format("##NeurodeInputRowCombo{}", selectedCol - 1).c_str(),
					nrdName[selectedRow[selectedCol - 1][NEURODE_BACKPROPAGATED_OUTPUT]].c_str(),
					ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_HeightLarge)) {
					for (int i = 0; i < nrdName.size(); ++i) {
						const bool is_selected = (selectedRow[selectedCol - 1][NEURODE_BACKPROPAGATED_OUTPUT] == i);

						if (ImGui::Selectable(nrdName[i].c_str(), is_selected))
							selectedRow[selectedCol - 1][NEURODE_BACKPROPAGATED_OUTPUT] = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				//Create neurode selection combobox
				ImGui::Text("Neurode Column: ");

				ImGui::SameLine();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo(std::format("##NeurodeInputNeurodeCombo{}", selectedCol - 1).c_str(),
					nrdName[selectedNrd[selectedCol - 1][NEURODE_BACKPROPAGATED_OUTPUT]].c_str(),
					ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_HeightLarge)) {
					for (int i = 0; i < nrdName.size(); ++i) {
						const bool is_selected = (selectedNrd[selectedCol - 1][NEURODE_BACKPROPAGATED_OUTPUT] == i);

						if (ImGui::Selectable(nrdName[i].c_str(), is_selected))
							selectedNrd[selectedCol - 1][NEURODE_BACKPROPAGATED_OUTPUT] = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				//Set table data at selected column
				for (int i = rows - 1; i > 0; --i)
					tableData[selectedCol - 1][i] = tableData[selectedCol - 1][i - 1];

				tableData[selectedCol - 1][0] = arti->neuralMatrix(nmName[selectedNrlMtx[selectedCol - 1]])->
					getNrdYpView().row(selectedRow[selectedCol - 1][NEURODE_BACKPROPAGATED_OUTPUT]).at(selectedNrd[selectedCol - 1][NEURODE_BACKPROPAGATED_OUTPUT]);
				break;
			case NEURODE_WEIGHTED_BACKPROPAGATED_OUTPUT:
				//Create neurode selection combobox
				ImGui::Text("Neurode Row: ");

				ImGui::SameLine();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo(std::format("##NeurodeInputRowCombo{}", selectedCol - 1).c_str(),
					nrdName[selectedRow[selectedCol - 1][NEURODE_WEIGHTED_BACKPROPAGATED_OUTPUT]].c_str(),
					ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_HeightLarge)) {
					for (int i = 0; i < nrdName.size(); ++i) {
						const bool is_selected = (selectedRow[selectedCol - 1][NEURODE_WEIGHTED_BACKPROPAGATED_OUTPUT] == i);

						if (ImGui::Selectable(nrdName[i].c_str(), is_selected))
							selectedRow[selectedCol - 1][NEURODE_WEIGHTED_BACKPROPAGATED_OUTPUT] = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				//Create neurode selection combobox
				ImGui::Text("Neurode Column: ");

				ImGui::SameLine();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo(std::format("##NeurodeInputNeurodeCombo{}", selectedCol - 1).c_str(),
					nrdName[selectedNrd[selectedCol - 1][NEURODE_WEIGHTED_BACKPROPAGATED_OUTPUT]].c_str(),
					ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_HeightLarge)) {
					for (int i = 0; i < nrdName.size(); ++i) {
						const bool is_selected = (selectedNrd[selectedCol - 1][NEURODE_WEIGHTED_BACKPROPAGATED_OUTPUT] == i);

						if (ImGui::Selectable(nrdName[i].c_str(), is_selected))
							selectedNrd[selectedCol - 1][NEURODE_WEIGHTED_BACKPROPAGATED_OUTPUT] = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				//Set table data at selected column
				for (int i = rows - 1; i > 0; --i)
					tableData[selectedCol - 1][i] = tableData[selectedCol - 1][i - 1];

				tableData[selectedCol - 1][0] = arti->neuralMatrix(nmName[selectedNrlMtx[selectedCol - 1]])->
					getNrdYpwView().row(selectedRow[selectedCol - 1][NEURODE_WEIGHTED_BACKPROPAGATED_OUTPUT]).
					at(selectedNrd[selectedCol - 1][NEURODE_WEIGHTED_BACKPROPAGATED_OUTPUT]);
				break;
			case OSCILLATOR_AMPLITUDE:
				//Create neurode selection combobox
				ImGui::Text("Neurode: ");

				ImGui::SameLine();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo(std::format("##OscillatorRowCombo{}", selectedCol - 1).c_str(),
					nrdName[selectedRow[selectedCol - 1][OSCILLATOR_AMPLITUDE]].c_str(),
					ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_HeightLarge)) {
					for (int i = 0; i < nrdName.size(); ++i) {
						const bool is_selected = (selectedRow[selectedCol - 1][OSCILLATOR_AMPLITUDE] == i);

						if (ImGui::Selectable(nrdName[i].c_str(), is_selected))
							selectedRow[selectedCol - 1][OSCILLATOR_AMPLITUDE] = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				//Set table data at selected column
				for (int i = rows - 1; i > 0; --i)
					tableData[selectedCol - 1][i] = tableData[selectedCol - 1][i - 1];

				tableData[selectedCol - 1][0] = arti->neuralMatrix(nmName[selectedNrlMtx[selectedCol - 1]])->
					getOscAView().at(selectedRow[selectedCol - 1][OSCILLATOR_AMPLITUDE]);
				break;
			case OSCILLATOR_FREQUENCY:
				//Create neurode selection combobox
				ImGui::Text("Neurode: ");

				ImGui::SameLine();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo(std::format("##OscillatorRowCombo{}", selectedCol - 1).c_str(),
					nrdName[selectedRow[selectedCol - 1][OSCILLATOR_FREQUENCY]].c_str(),
					ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_HeightLarge)) {
					for (int i = 0; i < nrdName.size(); ++i) {
						const bool is_selected = (selectedRow[selectedCol - 1][OSCILLATOR_FREQUENCY] == i);

						if (ImGui::Selectable(nrdName[i].c_str(), is_selected))
							selectedRow[selectedCol - 1][OSCILLATOR_FREQUENCY] = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				//Set table data at selected column
				for (int i = rows - 1; i > 0; --i)
					tableData[selectedCol - 1][i] = tableData[selectedCol - 1][i - 1];

				tableData[selectedCol - 1][0] = arti->neuralMatrix(nmName[selectedNrlMtx[selectedCol - 1]])->
					getOscFView().at(selectedRow[selectedCol - 1][OSCILLATOR_FREQUENCY]);
				break;
			case OSCILLATOR_PHASE:
				//Create neurode selection combobox
				ImGui::Text("Neurode: ");

				ImGui::SameLine();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo(std::format("##OscillatorRowCombo{}", selectedCol - 1).c_str(),
					nrdName[selectedRow[selectedCol - 1][OSCILLATOR_PHASE]].c_str(),
					ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_HeightLarge)) {
					for (int i = 0; i < nrdName.size(); ++i) {
						const bool is_selected = (selectedRow[selectedCol - 1][OSCILLATOR_PHASE] == i);

						if (ImGui::Selectable(nrdName[i].c_str(), is_selected))
							selectedRow[selectedCol - 1][OSCILLATOR_PHASE] = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				//Set table data at selected column
				for (int i = rows - 1; i > 0; --i)
					tableData[selectedCol - 1][i] = tableData[selectedCol - 1][i - 1];

				tableData[selectedCol - 1][0] = arti->neuralMatrix(nmName[selectedNrlMtx[selectedCol - 1]])->
					getOscPView().at(selectedRow[selectedCol - 1][OSCILLATOR_PHASE]);
				break;
			case OSCILLATOR_BASE_OUTPUT:
				//Create neurode selection combobox
				ImGui::Text("Neurode: ");

				ImGui::SameLine();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo(std::format("##OscillatorRowCombo{}", selectedCol - 1).c_str(),
					nrdName[selectedRow[selectedCol - 1][OSCILLATOR_BASE_OUTPUT]].c_str(),
					ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_HeightLarge)) {
					for (int i = 0; i < nrdName.size(); ++i) {
						const bool is_selected = (selectedRow[selectedCol - 1][OSCILLATOR_BASE_OUTPUT] == i);

						if (ImGui::Selectable(nrdName[i].c_str(), is_selected))
							selectedRow[selectedCol - 1][OSCILLATOR_BASE_OUTPUT] = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				//Set table data at selected column
				for (int i = rows - 1; i > 0; --i)
					tableData[selectedCol - 1][i] = tableData[selectedCol - 1][i - 1];

				tableData[selectedCol - 1][0] = arti->neuralMatrix(nmName[selectedNrlMtx[selectedCol - 1]])->
					getOscBView().at(selectedRow[selectedCol - 1][OSCILLATOR_BASE_OUTPUT]);
				break;
			case OSCILLATOR_OUTPUT:
				//Create neurode selection combobox
				ImGui::Text("Neurode: ");

				ImGui::SameLine();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo(std::format("##OscillatorRowCombo{}", selectedCol - 1).c_str(),
					nrdName[selectedRow[selectedCol - 1][OSCILLATOR_OUTPUT]].c_str(),
					ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_HeightLarge)) {
					for (int i = 0; i < nrdName.size(); ++i) {
						const bool is_selected = (selectedRow[selectedCol - 1][OSCILLATOR_OUTPUT] == i);

						if (ImGui::Selectable(nrdName[i].c_str(), is_selected))
							selectedRow[selectedCol - 1][OSCILLATOR_OUTPUT] = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				//Set table data at selected column
				for (int i = rows - 1; i > 0; --i)
					tableData[selectedCol - 1][i] = tableData[selectedCol - 1][i - 1];

				tableData[selectedCol - 1][0] = arti->neuralMatrix(nmName[selectedNrlMtx[selectedCol - 1]])->
					getOscYView().at(selectedRow[selectedCol - 1][OSCILLATOR_OUTPUT]);
				break;
			case NEURODE_INPUT_SUM:
				//Create neurode selection combobox
				ImGui::Text("Neurode: ");

				ImGui::SameLine();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo(std::format("##NeurodeRowCombo{}", selectedCol - 1).c_str(),
					nrdName[selectedRow[selectedCol - 1][NEURODE_INPUT_SUM]].c_str(),
					ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_HeightLarge)) {
					for (int i = 0; i < nrdName.size(); ++i) {
						const bool is_selected = (selectedRow[selectedCol - 1][NEURODE_INPUT_SUM] == i);

						if (ImGui::Selectable(nrdName[i].c_str(), is_selected))
							selectedRow[selectedCol - 1][NEURODE_INPUT_SUM] = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				//Set table data at selected column
				for (int i = rows - 1; i > 0; --i)
					tableData[selectedCol - 1][i] = tableData[selectedCol - 1][i - 1];

				tableData[selectedCol - 1][0] = arti->neuralMatrix(nmName[selectedNrlMtx[selectedCol - 1]])->
					getSView().at(selectedRow[selectedCol - 1][NEURODE_INPUT_SUM]);
				break;
			case NEURODE_INPUT_SUM_DECAY:
				//Create neurode selection combobox
				ImGui::Text("Neurode: ");

				ImGui::SameLine();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo(std::format("##NeurodeRowCombo{}", selectedCol - 1).c_str(),
					nrdName[selectedRow[selectedCol - 1][NEURODE_INPUT_SUM_DECAY]].c_str(),
					ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_HeightLarge)) {
					for (int i = 0; i < nrdName.size(); ++i) {
						const bool is_selected = (selectedRow[selectedCol - 1][NEURODE_INPUT_SUM_DECAY] == i);

						if (ImGui::Selectable(nrdName[i].c_str(), is_selected))
							selectedRow[selectedCol - 1][NEURODE_INPUT_SUM_DECAY] = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				//Set table data at selected column
				for (int i = rows - 1; i > 0; --i)
					tableData[selectedCol - 1][i] = tableData[selectedCol - 1][i - 1];

				tableData[selectedCol - 1][0] = arti->neuralMatrix(nmName[selectedNrlMtx[selectedCol - 1]])->
					getSdView().at(selectedRow[selectedCol - 1][NEURODE_INPUT_SUM_DECAY]);
				break;
			case NEURODE_ACTIVATION_HIGHPASS_PEAK:
				//Create neurode selection combobox
				ImGui::Text("Neurode: ");

				ImGui::SameLine();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo(std::format("##NeurodeRowCombo{}", selectedCol - 1).c_str(),
					nrdName[selectedRow[selectedCol - 1][NEURODE_ACTIVATION_HIGHPASS_PEAK]].c_str(),
					ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_HeightLarge)) {
					for (int i = 0; i < nrdName.size(); ++i) {
						const bool is_selected = (selectedRow[selectedCol - 1][NEURODE_ACTIVATION_HIGHPASS_PEAK] == i);

						if (ImGui::Selectable(nrdName[i].c_str(), is_selected))
							selectedRow[selectedCol - 1][NEURODE_ACTIVATION_HIGHPASS_PEAK] = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				//Set table data at selected column
				for (int i = rows - 1; i > 0; --i)
					tableData[selectedCol - 1][i] = tableData[selectedCol - 1][i - 1];

				tableData[selectedCol - 1][0] = arti->neuralMatrix(nmName[selectedNrlMtx[selectedCol - 1]])->
					getAhView().at(selectedRow[selectedCol - 1][NEURODE_ACTIVATION_HIGHPASS_PEAK]);
				break;
			case NEURODE_ACTIVATION_LOWPASS_PEAK:
				//Create neurode selection combobox
				ImGui::Text("Neurode: ");

				ImGui::SameLine();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo(std::format("##NeurodeRowCombo{}", selectedCol - 1).c_str(),
					nrdName[selectedRow[selectedCol - 1][NEURODE_ACTIVATION_LOWPASS_PEAK]].c_str(),
					ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_HeightLarge)) {
					for (int i = 0; i < nrdName.size(); ++i) {
						const bool is_selected = (selectedRow[selectedCol - 1][NEURODE_ACTIVATION_LOWPASS_PEAK] == i);

						if (ImGui::Selectable(nrdName[i].c_str(), is_selected))
							selectedRow[selectedCol - 1][NEURODE_ACTIVATION_LOWPASS_PEAK] = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				//Set table data at selected column
				for (int i = rows - 1; i > 0; --i)
					tableData[selectedCol - 1][i] = tableData[selectedCol - 1][i - 1];

				tableData[selectedCol - 1][0] = arti->neuralMatrix(nmName[selectedNrlMtx[selectedCol - 1]])->
					getAlView().at(selectedRow[selectedCol - 1][NEURODE_ACTIVATION_LOWPASS_PEAK]);
				break;
			case NEURODE_ACTIVATION_SENSITIVITY:
				//Create neurode selection combobox
				ImGui::Text("Neurode: ");

				ImGui::SameLine();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo(std::format("##NeurodeRowCombo{}", selectedCol - 1).c_str(),
					nrdName[selectedRow[selectedCol - 1][NEURODE_ACTIVATION_SENSITIVITY]].c_str(),
					ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_HeightLarge)) {
					for (int i = 0; i < nrdName.size(); ++i) {
						const bool is_selected = (selectedRow[selectedCol - 1][NEURODE_ACTIVATION_SENSITIVITY] == i);

						if (ImGui::Selectable(nrdName[i].c_str(), is_selected))
							selectedRow[selectedCol - 1][NEURODE_ACTIVATION_SENSITIVITY] = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				//Set table data at selected column
				for (int i = rows - 1; i > 0; --i)
					tableData[selectedCol - 1][i] = tableData[selectedCol - 1][i - 1];

				tableData[selectedCol - 1][0] = arti->neuralMatrix(nmName[selectedNrlMtx[selectedCol - 1]])->
					getAsView().at(selectedRow[selectedCol - 1][NEURODE_ACTIVATION_SENSITIVITY]);
				break;
			case NEURODE_ACTIVATION:
				//Create neurode selection combobox
				ImGui::Text("Neurode: ");

				ImGui::SameLine();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo(std::format("##NeurodeRowCombo{}", selectedCol - 1).c_str(),
					nrdName[selectedRow[selectedCol - 1][NEURODE_ACTIVATION]].c_str(),
					ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_HeightLarge)) {
					for (int i = 0; i < nrdName.size(); ++i) {
						const bool is_selected = (selectedRow[selectedCol - 1][NEURODE_ACTIVATION] == i);

						if (ImGui::Selectable(nrdName[i].c_str(), is_selected))
							selectedRow[selectedCol - 1][NEURODE_ACTIVATION] = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				//Set table data at selected column
				for (int i = rows - 1; i > 0; --i)
					tableData[selectedCol - 1][i] = tableData[selectedCol - 1][i - 1];

				tableData[selectedCol - 1][0] = arti->neuralMatrix(nmName[selectedNrlMtx[selectedCol - 1]])->
					getAView().at(selectedRow[selectedCol - 1][NEURODE_ACTIVATION]);
				break;
			case NEURODE_THRESHOLD:
				//Create neurode selection combobox
				ImGui::Text("Neurode: ");

				ImGui::SameLine();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo(std::format("##NeurodeRowCombo{}", selectedCol - 1).c_str(),
					nrdName[selectedRow[selectedCol - 1][NEURODE_THRESHOLD]].c_str(),
					ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_HeightLarge)) {
					for (int i = 0; i < nrdName.size(); ++i) {
						const bool is_selected = (selectedRow[selectedCol - 1][NEURODE_THRESHOLD] == i);

						if (ImGui::Selectable(nrdName[i].c_str(), is_selected))
							selectedRow[selectedCol - 1][NEURODE_THRESHOLD] = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				//Set table data at selected column
				for (int i = rows - 1; i > 0; --i)
					tableData[selectedCol - 1][i] = tableData[selectedCol - 1][i - 1];

				tableData[selectedCol - 1][0] = arti->neuralMatrix(nmName[selectedNrlMtx[selectedCol - 1]])->
					getTView().at(selectedRow[selectedCol - 1][NEURODE_THRESHOLD]);
				break;
			case NEURODE_REFRACTORY_PERIOD:
				//Create neurode selection combobox
				ImGui::Text("Neurode: ");

				ImGui::SameLine();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo(std::format("##NeurodeRowCombo{}", selectedCol - 1).c_str(),
					nrdName[selectedRow[selectedCol - 1][NEURODE_REFRACTORY_PERIOD]].c_str(),
					ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_HeightLarge)) {
					for (int i = 0; i < nrdName.size(); ++i) {
						const bool is_selected = (selectedRow[selectedCol - 1][NEURODE_REFRACTORY_PERIOD] == i);

						if (ImGui::Selectable(nrdName[i].c_str(), is_selected))
							selectedRow[selectedCol - 1][NEURODE_REFRACTORY_PERIOD] = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				//Set table data at selected column
				for (int i = rows - 1; i > 0; --i)
					tableData[selectedCol - 1][i] = tableData[selectedCol - 1][i - 1];

				tableData[selectedCol - 1][0] = arti->neuralMatrix(nmName[selectedNrlMtx[selectedCol - 1]])->
					getRView().at(selectedRow[selectedCol - 1][NEURODE_REFRACTORY_PERIOD]);
				break;
			case NEURODE_OUTPUT:
				//Create neurode selection combobox
				ImGui::Text("Neurode: ");

				ImGui::SameLine();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo(std::format("##NeurodeRowCombo{}", selectedCol - 1).c_str(),
					nrdName[selectedRow[selectedCol - 1][NEURODE_OUTPUT]].c_str(),
					ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_HeightLarge)) {
					for (int i = 0; i < nrdName.size(); ++i) {
						const bool is_selected = (selectedRow[selectedCol - 1][NEURODE_OUTPUT] == i);

						if (ImGui::Selectable(nrdName[i].c_str(), is_selected))
							selectedRow[selectedCol - 1][NEURODE_OUTPUT] = i;

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				//Set table data at selected column
				for (int i = rows - 1; i > 0; --i)
					tableData[selectedCol - 1][i] = tableData[selectedCol - 1][i - 1];

				tableData[selectedCol - 1][0] = arti->neuralMatrix(nmName[selectedNrlMtx[selectedCol - 1]])->
					getYView().at(selectedRow[selectedCol - 1][NEURODE_OUTPUT]);
				break;
			default:
				break;
			}
		}

		ImGui::EndChild();

		//If columns exist
		if (columns > 0) {
			//Create table region
			ImGui::SameLine();
			ImGui::BeginChild("Table Region", ImVec2(0.0f, 0.0f), false, ImGuiWindowFlags_HorizontalScrollbar);

			//Create table
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (ImGui::GetContentRegionAvail().x / 2) - (ImGui::CalcTextSize(title.c_str()).x / 2));
			ImGui::Text(title.c_str());

			if (ImGui::BeginTable("##NeuralTable", columns, tableFlags, ImVec2(100.0f * columns, ImGui::GetContentRegionAvail().y))) {
				//Create column headers
				for (int i = 0; i < columns; ++i) {
					ImGui::TableSetupColumn(header[i].c_str(), ImGuiTableColumnFlags_WidthStretch, 100.0f);
				}
				ImGui::TableHeadersRow();

				//Create column data
				for (int i = 0; i < rows; ++i) {
					ImGui::TableNextRow();

					for (int j = 0; j < columns; ++j) {
						ImGui::TableNextColumn();
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - 
							ImGui::CalcTextSize(std::format("{:.3f}", tableData[j][i]).c_str()).x);
						ImGui::Text(std::format("{:.3f}", tableData[j][i]).c_str());
					}
				}

				ImGui::EndTable();
			}

			ImGui::EndChild();
		}

		ImGui::End();
	}
}