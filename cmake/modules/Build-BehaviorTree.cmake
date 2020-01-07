set(BEHAVIOR_TREE_INSTALL_DIR ${CMAKE_THIRD_PARTY_DIR})

set(_byproducts
    ${BEHAVIOR_TREE_INSTALL_DIR}/lib/behaviortree_cpp_v3.dll
    ${BEHAVIOR_TREE_INSTALL_DIR}/lib/libbehaviortree_cpp_v3.so
  )

ExternalProject_Add(
  behavior_tree
  GIT_REPOSITORY https://github.com/BehaviorTree/BehaviorTree.CPP
  GIT_TAG 0ab389505deae351feed6f666fa3a3e083d02339
  CMAKE_ARGS -G ${CMAKE_GENERATOR} -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DCMAKE_INSTALL_PREFIX=${BEHAVIOR_TREE_INSTALL_DIR} -DCMAKE_MODULE_PATH=${CMAKE_MODULE_PATH} -DBUILD_UNIT_TESTS=OFF -DBUILD_EXAMPLES=OFF -DBUILD_TOOLS=OFF
  BUILD_BYPRODUCTS ${_byproducts}
  INSTALL_DIR ${BEHAVIOR_TREE_INSTALL_DIR}
  PATCH_COMMAND ${PATCH_SCRIPT_PATH} ${CMAKE_PATCH_DIR}/behavior_tree.patch
)

ExternalProject_Get_Property(
  behavior_tree
  install_dir
)

set(BEHAVIOR_TREE_INCLUDE_DIR "${install_dir}/include/behaviortree_cpp_v3")
if(NOT MINGW AND WIN32)
    set(BEHAVIOR_TREE_LIBRARIES "${install_dir}/lib/behaviortree_cpp_v3.lib")
else()
    set(BEHAVIOR_TREE_LIBRARIES "${install_dir}/lib/libbehaviortree_cpp_v3.so")
endif()

if(NOT TARGET BT::behavior_tree)
    add_library(BT::behavior_tree INTERFACE IMPORTED)
    add_dependencies(BT::behavior_tree behavior_tree)
    set_target_properties(BT::behavior_tree PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${BEHAVIOR_TREE_INCLUDE_DIR}")
    set_target_properties(BT::behavior_tree PROPERTIES INTERFACE_LINK_LIBRARIES "${BEHAVIOR_TREE_LIBRARIES}")
endif()
