#!/bin/bash

#-- I Need to touch version.cpp to trigger a build
#-- with the new build date :( !!
touch ../version.cpp
BUILD_DATE=`date +'%F %T'`
echo -e "#ifndef BUILD_DATE_H\n#define BUILD_DATE_H\n#define BUILD_DATE \"${BUILD_DATE}\"\n#endif" > build_date.h
