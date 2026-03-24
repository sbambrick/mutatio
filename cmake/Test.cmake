if(NOT TARGET test_bin)
  add_custom_target(test_bin COMMENT "Build test complete.")
endif()

if(NOT TARGET test)
  add_custom_target(test COMMENT "Test complete.")
endif()

function(add_test_target tgt_name tgt_dir tgt_test_deps)
  set(tgt_name_test     "${tgt_name}_test")
  set(tgt_name_test_bin "${tgt_name}_test_bin")

  file(GLOB tgt_src CONFIGURE_DEPENDS ${tgt_dir}/tests/*.cc)
  file(GLOB tgt_hdr CONFIGURE_DEPENDS ${tgt_dir}/tests/*.h)

  if(tgt_src)
    add_executable(${tgt_name_test_bin} ${tgt_src} ${tgt_hdr})
    target_include_directories(${tgt_name_test_bin} PUBLIC ${tgt_dir})
    target_link_libraries(
      ${tgt_name_test_bin} PUBLIC
      ${tgt_name}
      ${tgt_test_deps}
      gtest::gtest
    )

    add_custom_target(
      ${tgt_name_test}
      COMMAND find ${CMAKE_BINARY_DIR} -name "*.gcda" -delete
      COMMAND ${tgt_name_test_bin}
    )
    add_dependencies(test_bin ${tgt_name_test_bin})
    add_dependencies(test ${tgt_name_test})
  endif()
endfunction()
