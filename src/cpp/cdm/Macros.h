/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

#define MIN(A,B)                (((A) < (B)) ? (A) : (B))
#define MAX(A,B)                (((A) > (B)) ? (A) : (B))
#define BLIM(input,min,max)     (input = (input>max)?max:((input<min)?min:input))
#define LLIM(input,min)         (input = (input<min)?min:input)
#define ULIM(input,max)         (input = (input>max)?max:input)
#define LIMIT(A,B,C)            (((A) < (B)) ? (B) : (((A) > (C)) ? (C) : (A)))

template <class _InIt>
inline size_t _Hash_value(_InIt _Begin, _InIt _End)
{  // hash range of elements
  size_t _Val = 2166136261U;

  while (_Begin != _End)
    _Val = 16777619U * _Val ^ (size_t)*_Begin++;
  return (_Val);
}

/* The following warnings have been disabled while building the protobuf libraries and compiler. You may have to disable some of them in your own project as well, or live with them.
    C4018 - 'expression' : signed/unsigned mismatch
    C4146 - unary minus operator applied to unsigned type, result still unsigned
    C4244 - Conversion from 'type1' to 'type2', possible loss of data.
    C4251 - 'identifier' : class 'type' needs to have dll-interface to be used by clients of class 'type2'
    C4267 - Conversion from 'size_t' to 'type', possible loss of data.
    C4305 - 'identifier' : truncation from 'type1' to 'type2'
    C4355 - 'this' : used in base member initializer list
    C4800 - 'type' : forcing value to bool 'true' or 'false' (performance warning)
    C4996 - 'function': was declared deprecated
*/

#define DEFINE_STATIC_STRING(name) static constexpr char const* name = #name;
#define DEFINE_STATIC_STRING_EX(name,value) static constexpr char const* name = #value;

#define SAFE_DELETE(obj) {delete obj; obj=nullptr;}

#define SAFE_DELETE_ARRAY(ary) {delete [] ary; *ary=nullptr;}

#define INVALIDATE_PROPERTY(name) \
  if(name!=nullptr) \
    name->Invalidate();

#define FORCE_INVALIDATE_PROPERTY(name) \
  if(name!=nullptr) \
    name->ForceInvalidate();

#define ZERO_SCALAR(name) \
  if(name!=nullptr && name->IsValid()) \
    name->SetValue(0);

#define ZERO_UNIT_SCALAR(name) \
  if(name!=nullptr && name->IsValid()) \
    name->SetValue(0,*name->GetUnit());

#define COPY_PROPERTY(name) \
  if(from.Has##name()) \
    Get##name().Set(*from.m_##name);

#define MERGE_CHILD_SUBS(name, subMgr) \
  if(from.Has##name()) \
    Get##name().Merge(*from.m_##name, subMgr);

#define MERGE_CHILD(name) \
  if(from.Has##name()) \
    Get##name().Merge(*from.m_##name);

#define CDM_COPY(clazz, from, to) \
  { \
    auto* bind = clazz ::Unload(*from); \
    clazz ::Load(*bind,*to); \
    delete bind; \
  }

template<class T>
inline void Copy(const std::vector<T*>& from, std::vector<T*>& to)
{
  to.clear();
  to.resize(from.size());
  std::copy(from.begin(), from.end(), to.begin());
}

template<class T>
inline void Copy(const std::vector<T>& from, std::vector<T>& to)
{
  to.clear();
  to.resize(from.size());
  std::copy(from.begin(), from.end(), to.begin());
}

template<class T>
inline bool Contains(const std::vector<T*>& v, T& item)
{
  return std::find(v.begin(), v.end(), &item) != v.end();
}

template<class T>
inline void Remove(std::vector<T*>& v, T* item)
{
  v.erase(std::remove(v.begin(), v.end(), item), v.end());
}

template<class T>
inline void Copy(const std::vector<T*>& from, std::vector<const T*>& to)
{
  to.clear();
  for (auto i : from)
    to.push_back(i);
}

template<class T>
inline void DELETE_VECTOR(std::vector<T>& vec) 
{
  for(unsigned int i=0; i<vec.size(); i++)
  {
    delete vec.at(i);
  }
  vec.clear();
}
//This will delete all of the second items in a map and then clear the map to destroy all dangling pointers
template<class T, class K>
inline void DELETE_MAP_SECOND(std::map<T,K>& map) 
{
  for(typename std::map<T,K>::iterator it=map.begin(); it != map.end(); ++it)
  {
    delete it->second;
  }
  map.clear();
}

template <typename valueType>
bool SameSign(valueType x, valueType y)
{
  return (x >= 0) ^ (y < 0);
}
