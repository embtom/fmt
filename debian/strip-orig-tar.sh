#!/bin/bash

exclude_patterns=(
	'*/gmock/' '*/gtest/' '*/gmock-gtest-all.cc' # embedded GTest and GMock libraries
	'*Android*'
	'*appveyor*'
	'*travis*'
	'*/support/rtd'
	'*/support/*.py'
	'*/doc/bootstrap' # custom theme
	'*/doc/python-license.txt'
	'*/_static/fonts' # custom fonts
	'*/_static/bootstrap*js' # sourceless JavaScript
)

gunzip -c | tar --wildcards --delete "${exclude_patterns[@]}" | gzip -c --best

