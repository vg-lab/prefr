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
      this->size = _start - _end;
    }

    ElementCollection(vector<ELEM*>* elementsArray, typename vector<ELEM*>::iterator _start, typename vector<ELEM*>::iterator _end)
    {
      this->elements = elementsArray;
      this->start = _start;
      this->end = _end;
      this->size = _start - _end;
    }

    ELEM* GetElement (unsigned int i)
    {
      return this->elements->at(this->start + i);
    }

  };

}

#endif
