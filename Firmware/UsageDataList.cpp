#include "UsageDataList.h"

UsageDataList::UsageDataList()
{
	head = nullptr;
	tail = nullptr;
}

UsageDataList::~UsageDataList()
{
	UsageDataNode* cursor = head;
	while (head) {
		cursor = cursor->getNext();
		delete head;
		head = cursor;
	}
	head = nullptr; // Officially empty
}

void UsageDataList::insertBeforeFirst(char newEvent, int newYear, int newMonth, int newDay, int newHour, int newMinute, int newSecond)
{
	head = new UsageDataNode(newEvent, newYear, newMonth, newDay, newHour, newMinute, newSecond, head);
	if (tail == nullptr)
	{
		tail = head;
	}
}

void UsageDataList::insertAfterLast(char newEvent, int newYear, int newMonth, int newDay, int newHour, int newMinute, int newSecond)
{
	tail->setNext(new UsageDataNode(newEvent, newYear, newMonth, newDay, newHour, newMinute, newSecond, nullptr));
	tail = tail->getNext();
	if (head == nullptr)
	{
		head = tail;
	}
}

void UsageDataList::removeFirst()
{
	if (head != nullptr) {
		UsageDataNode* oldHead = head;
		head = head->getNext();
		delete oldHead;
	}
}


void UsageDataList::listAll()
{
	UsageDataNode* aux = head;
	while (aux != nullptr) {
		aux->serialPrintNode();
		aux = aux->getNext();
	}
}
