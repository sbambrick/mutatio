function(add_module_target tgt_name tgt_dir tgt_deps tgt_test_deps)
  set(tgt_name_lib ${tgt_name})

  file(GLOB tgt_src CONFIGURE_DEPENDS ${tgt_dir}/src/*.cc)
  file(GLOB tgt_hdr CONFIGURE_DEPENDS ${tgt_dir}/include/*/*.h)

  set(main_path ${tgt_dir}/src/main.cc)
  if(EXISTS ${main_path})
    list(REMOVE_ITEM tgt_src ${main_path})
  endif()

  if(tgt_src)
    add_library(${tgt_name_lib} STATIC ${tgt_src} ${tgt_hdr})
    target_include_directories(${tgt_name_lib} PUBLIC ${tgt_dir}/include)
    target_link_libraries(${tgt_name_lib} PUBLIC ${tgt_deps})
  else()
    add_library(${tgt_name_lib} INTERFACE ${tgt_src} ${tgt_hdr})
    target_include_directories(${tgt_name_lib} INTERFACE ${tgt_dir}/include)
    target_link_libraries(${tgt_name_lib} INTERFACE ${tgt_deps})
  endif()

  add_library(${CMAKE_PROJECT_NAME}::${tgt_name_lib} ALIAS ${tgt_name_lib})

  add_format_target(${tgt_name} ${tgt_dir})
  
  if(ENABLE_TESTING)
    add_test_target(${tgt_name} "${tgt_dir}" "${tgt_test_deps}")
  endif()
endfunction()
