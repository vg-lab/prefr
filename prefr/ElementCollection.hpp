#ifndef ELEMENT_COLLECTION_H_
#define ELEMENT_COLLECTION_H_


#include <vector>

using namespace std;

namespace utils
{

  template <class ELEM>
  class ElementCollection
  {
  public:

    vector<ELEM*>* elements;
    typename vector<ELEM*>::iterator start;
    typename vector<ELEM*>::iterator end;

    int size;

    ElementCollection(vector<ELEM*>* elementsArray, unsigned int _start, unsigned int _end)
    {
      this->elements = elementsArray;
      this->start = elementsArray->begin() + _start;
      this->end = elementsArray->begin() + _end;
      this->size = _end - _start;
    }

    ElementCollection(vector<ELEM*>* elementsArray, typename vector<ELEM*>::iterator _start, typename vector<ELEM*>::iterator _end)
    {
      this->elements = elementsArray;
      this->start = _start;
      this->end = _end;
      this->size = _end - _start;
    }

    ElementCollection(const ElementCollection& other)
    {
      this->elements = other.elements;
      this->start = other.start;
      this->end = other.end;
      this->size = this->end - this->start;
    }

    ElementCollection(const ElementCollection* other)
    {
      this->elements = other->elements;
      this->start = other->start;
      this->end = other->end;
      this->size = this->end - this->start;
    }

    ElementCollection(ElementCollection* other, unsigned int _start, unsigned int _end)
    {
      this->elements = other->elements;
      this->start = other->elements->begin() + _start;
      this->end = other->elements->begin() + _end;
      this->size = _end - _start;
    }

    ElementCollection(ElementCollection* other, typename vector<ELEM*>::iterator _start, typename vector<ELEM*>::iterator _end)
    {
      this->elements = other->elements;
      this->start = _start;
      this->end = _end;
      this->size = _end - _start;
    }

    ELEM* GetElementLocal (unsigned int i)
    {
      return this->elements->at(this->start + i);
    }

    ELEM* GetElement( unsigned int i)
    {
      return this->elements->at(i);
    }

  };

}

#endif
