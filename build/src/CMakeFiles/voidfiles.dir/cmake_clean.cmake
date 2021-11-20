file(REMOVE_RECURSE
  "libvoidfiles.a"
  "libvoidfiles.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/voidfiles.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
