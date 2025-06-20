if(NOT TARGET format)
  add_custom_target(format COMMENT "Format complete.")
endif()

if(NOT TARGET check_format)
  add_custom_target(check_format COMMENT "Check format complete.")
endif()

function(add_format_target tgt_name tgt_dir)
  set(tgt_name_format "${tgt_name}_format")
  add_custom_target(
    ${tgt_name_format}
    COMMAND find . -iname *.h -o -iname *.cc | xargs clang-format -i
    WORKING_DIRECTORY ${tgt_dir}
  )
  add_dependencies(format ${tgt_name_format})
endfunction()
