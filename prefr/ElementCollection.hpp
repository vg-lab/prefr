#ifndef ELEMENT_COLLECTION_H_
#define ELEMENT_COLLECTION_H_


#include <vector>

namespace utils
{

  template <class ELEM>
  class ElementCollection
  {
  public:

    std::vector<ELEM*>* elements;
    typename std::vector<ELEM*>::iterator start;
    typename std::vector<ELEM*>::iterator end;

    int size;

    ElementCollection(std::vector<ELEM*>* elementsArray, unsigned int _start, unsigned int _end)
    {
      this->elements = elementsArray;
      this->start = elementsArray->begin() + _start;
      this->end = elementsArray->begin() + _end;
      this->size = _end - _start;
    }

    ElementCollection(std::vector<ELEM*>* elementsArray, typename std::vector<ELEM*>::iterator _start, typename std::vector<ELEM*>::iterator _end)
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

    ElementCollection(ElementCollection* other, typename std::vector<ELEM*>::iterator _start, typename std::vector<ELEM*>::iterator _end)
    {
      this->elements = other->elements;
      this->start = _start;
      this->end = _end;
      this->size = _end - _start;
    }

    inline ELEM* GetElementLocal (unsigned int i)
    {
      return (*this->elements)[this->start + i];
    }

    inline ELEM* GetElement( unsigned int i)
    {
      return (*this->elements)[i];
    }

  };

}

#endif
