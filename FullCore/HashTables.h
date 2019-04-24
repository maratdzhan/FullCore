#pragma once


class Node
{
public:
	Node()
	{
		next = NULL;
		itemIndex = -1;

	}

	Node * next;
	int itemIndex;
	std::string itemData;
};

struct ConstantsList
{
	ConstantsList()
	{
		this->_length = 0;
		this->head = NULL;
	}

	void Add(int _index, const std::string & _data)
	{

		Node* node = new Node();
		node->itemData = _data;
		node->itemIndex = _index;
		node->next = this->head;
		this->head = node;
		this->_length++;
	}

	int Find(const std::string & _name) const
	{
		Node * head = this->head;
		while (head)
		{
			if (head->itemData == _name)
				return head->itemIndex;
			head = head->next;
		}

		return -1;
	}

	void PrintList() const
	{
		Node* head = this->head;
		int i = 1;
		while (head) 
		{
			std::cerr << i << ": " << head->itemData << std::endl;
			head = head->next;
			i++;
		}
	}

private:
	int _length;
	Node * head;
};



class ConstantsHashTable
{
public:
	ConstantsHashTable()
	{
		_hashSize = 150;
		_constants.resize(_hashSize);
	}

	~ConstantsHashTable()
	{

	}
	
	int MakeHashCode(const std::string & _cstr) const
	{
		int sumC = 0;
		int sumN = 0;
		for (const auto & _item : _cstr)
		{
			if (IsItemDigit(_item))
				sumN += char(_item);
			else
				sumC += char(_item);
		}

		return (_cstr.size()* (sumN*sumC));
	}

	bool IsItemDigit(const char item) const
	{
		
		if (char(item) >= 48 && char(item) <= 57)
			return true;
		return false;
	}

	int GetIndex(int _hashCode) const
	{
		return (_hashCode % _hashSize);
	}

	void Put(std::string & _value, int _index)
	{
		int _l_hash = (MakeHashCode(_value));
		_constants[GetIndex(_l_hash)].Add(_index, _value);
	}

	int Get(const std::string & _value) const
	{
		return (_constants[GetIndex(MakeHashCode(_value))].Find(_value));
	
	}

	void PrintAll()
	{
		for (const auto & _item : _constants)
			_item.PrintList();
	}

private:
	std::vector<ConstantsList> _constants;
	int _hashSize;
};
