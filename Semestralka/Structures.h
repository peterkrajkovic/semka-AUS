#pragma once
#include <libds/heap_monitor.h>
#include <fstream>
#include <string>
#include "Territory.h"
#include <libds/amt/hierarchy.h>
#include "libds/adt/table.h"
#include <libds/amt/explicit_hierarchy.h>
class Structures {
private:
	ds::amt::MultiWayExplicitHierarchy<Territory*> hierarchy;
	ds::adt::HashTable<std::string, ds::amt::ImplicitSequence<Territory*>*> towns;
	ds::adt::HashTable<std::string, ds::amt::ImplicitSequence<Territory*>*> districts;
	ds::adt::HashTable<std::string, ds::amt::ImplicitSequence<Territory*>*> regions;
	ds::amt::ImplicitSequence<Territory*> sequence;

public:
	void load() {
		//creating the structures
		std::string number, code, officialTitle, mediumTitle, shortTitle, note,line,word;
		ds::amt::ImplicitSequence<std::string> row;
		ds::amt::ImplicitSequence<std::string> row2;
		std::string names[] = { "kraje.csv","okresy.csv","obce.csv","kraje2.csv","okresy2.csv","obce2.csv" };
		std::string types[] = { "kraj","okres","obec" };
		std::ifstream file;
		std::ifstream file2;
		hierarchy.emplaceRoot();
		char dist = '1';
	    auto territory = new Territory(0, "SVK", "Slovenská republika", "Slovensko", "Slovensko", "SK","krajina");
		territory->setNumberOfCitizens(5449270);
		hierarchy.accessRoot()->data_ = territory;
		for (int i = 0; i < 3; i++) {
			file.open(names[i], std::ios::in);
			file2.open(names[i+3], std::ios::in);
			if (file.is_open())
			{
				std::string type = types[i];
				getline(file, line);
				getline(file2, line);
				int sonOrder = 0;
				int fatherOrder = 0;
				int grandFatherOrder = 0;
				int numberOfCitizens = 0;
				std::string code = "10";
				while (getline(file, line))
				{
					//reading line
					row.clear();
					row2.clear();
					std::stringstream str(line);
					while (std::getline(str, word, ';')) {
						row.insertLast().data_= word;
					}
					word = "";
					if (row.size() != 6) {
						row.insertLast().data_ = word;
					}
					territory = new Territory(stoi(row.access(0)->data_), row.access(1)->data_, row.access(2)->data_, row.access(3)->data_, row.access(4)->data_, row.access(5)->data_, type);

					//loading number of citizens
					if (getline(file2, line)) {
						std::stringstream str(line);
						while (std::getline(str, word, ',')) {
							row2.insertLast().data_ = word;
						}
					}
					if (row2.size() > 2) {
						numberOfCitizens = stoi(row2.access(2)->data_);
						territory->setNumberOfCitizens(numberOfCitizens);
					}

					//input of district
					if (i > 0 && i < 2) {
						if (row.access(1)->data_.substr(3, 2) != code) {
							code = row.access(1)->data_.substr(3, 2);
							fatherOrder++;
							sonOrder = 0;
						}
						hierarchy.emplaceSon(*hierarchy.accessSon(*hierarchy.accessRoot(), fatherOrder), sonOrder).data_ = territory;
					}
					//input of towns
					else if (i == 2) {
						if (row.access(1)->data_[5] != dist) {
							dist = row.access(1)->data_[5];
							fatherOrder++;
							sonOrder = 0;
						}
						if (row.access(1)->data_.substr(3, 2) != code) {
							code = row.access(1)->data_.substr(3, 2);
							grandFatherOrder++;
							fatherOrder = 0;
							sonOrder = 0;
						}
						hierarchy.emplaceSon(*hierarchy.accessSon(*hierarchy.accessSon(*hierarchy.accessRoot(), grandFatherOrder), fatherOrder), sonOrder).data_ = territory;
						}
					else {
						//input of regions
						hierarchy.emplaceSon(*hierarchy.accessRoot(), sonOrder).data_ = territory;
					}
					sonOrder++;
				}
				file.close();
				file2.close();
			}
		}
		loadTable();
	}
	void loadTable() {
		//process every territory in hierarchy then load to its table
		hierarchy.processPostOrder(hierarchy.accessRoot(), [&](ds::amt::MultiWayExplicitHierarchyBlock<Territory*>* t) {
			if (t->data_->getType() == "obec") {
				if (towns.contains(t->data_->getOfficial())) {
					towns.find(t->data_->getOfficial())->insertLast().data_ = t->data_;
				}
				else {
					auto data = new ds::amt::ImplicitSequence<Territory*>();
					data->insertFirst().data_ = t->data_;
					towns.insert(t->data_->getOfficial(), data);
				}
			}
			else if (t->data_->getType() == "okres") {
				if (districts.contains(t->data_->getOfficial())) {
					districts.find(t->data_->getOfficial())->insertLast().data_ = t->data_;
				}
				else {
					auto data = new ds::amt::ImplicitSequence<Territory*>();
					data->insertFirst().data_ = t->data_;
					districts.insert(t->data_->getOfficial(), data);
				}
			}
			else if (t->data_->getType() == "kraj") {
				if (regions.contains(t->data_->getOfficial())) {
					regions.find(t->data_->getOfficial())->insertLast().data_ = t->data_;
				}
				else {
					auto data = new ds::amt::ImplicitSequence<Territory*>();
					data->insertFirst().data_ = t->data_;
					regions.insert(t->data_->getOfficial(), data);
				}
			}
		});
	}
	~Structures() {
		//deleting data in all structures
		for (auto& x : towns) {
			delete x.data_;
		}
		for (auto& x : districts) {
			delete x.data_;
		}
		for (auto& x : regions) {
			delete x.data_;
		}
		hierarchy.processPostOrder(hierarchy.accessRoot(), [&](ds::amt::MultiWayExplicitHierarchyBlock<Territory*>* t) {
			delete t->data_;
		});
	}
	ds::amt::MultiWayExplicitHierarchy<Territory*> getHierarchy() {
		return hierarchy;
	}
	ds::adt::HashTable<std::string, ds::amt::ImplicitSequence<Territory*>*> getTowns() {
		return towns;
	}
	ds::adt::HashTable<std::string, ds::amt::ImplicitSequence<Territory*>*> getRegions() {
		return regions;
	}
	ds::adt::HashTable<std::string, ds::amt::ImplicitSequence<Territory*>*> getDistricts() {
		return districts;
	}
	ds::amt::ImplicitSequence<Territory*> getSequence() {
		return sequence;
	}
};