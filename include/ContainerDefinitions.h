/*
*  FILE          ContainerDefinitions.h
*  AUTHOR        Brendon Clifford
*  DESCRIPTION
*    Declaration for forward declaration container types.
*/
#pragma once

#include <list>
#include <vector>
#include <unordered_map>
#include <map>
#include <tuple>
#include <array>
#include <stack>
#include <initializer_list>

namespace Containers
{
  template<class T>
  using List = std::list<T>;

  template<class T>
  using Vector = std::vector<T>;

  template<class Key, class T>
  using UnorderedMap = std::unordered_map<Key, T>;

  template<class Key, class T, class Hash>
  using UnorderedMapUserHash = std::unordered_map<Key, T, Hash>;

  template<class Key, class T>
  using Map = std::map<Key, T>;

  template<class T, size_t size>
  using Array = std::array<T, size>;

  template<class... arguments>
  using Tuple = std::tuple<arguments...>;

  template<size_t N, class T>
  using TupleElement = std::tuple_element<N, T>;

  template<class T>
  using Stack = std::stack<T>;

  template<class T>
  using InitializerList = std::initializer_list<T>;
}
