#!/bin/sh
if ! which logkeys ; then
	PATH="${GOPATH}/bin:$PATH"
fi
if [ "$DEBUG_AS_ROOT" = "true" ]; then
	exec sudo logkeys "$@"
else
	exec logkeys "$@"
fi