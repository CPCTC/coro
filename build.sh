compile () {
	local file="$1"
	shift

	local bd="${BUILD_DIR}/`dirname "$file"`"
	case "$file" in
		*.c)
			STEP mkdir -p "$bd"
			STEP $CACHE $CC $FLAGS -c $CFLAGS \
				-o "${BUILD_DIR}/${file}.o" "$file";;
		*.cpp)
			STEP mkdir -p "$bd"
			STEP $CACHE $CXX $FLAGS -c $CFLAGS \
				-o "${BUILD_DIR}/${file}.o" "$file";;
	esac
}

test () {
	local test="$1"
	shift

	[[ "$test" == *.c ]] || [[ "$test" == *.cpp ]] || return 0

	compile "$test"
	STEP $CXX $FLAGS \
		-o "${BUILD_DIR}/$test" \
		"${BUILD_DIR}/$test.o" \
		"${BUILD_DIR}/${NAME}.o" \
		$LFLAGS
	STEP TEST "./${BUILD_DIR}/$test"
}

link () {
	local objs=`find "$BUILD_DIR" -name '*.o'`
	case "$TYPE" in
		bin)
			STEP $CXX $FLAGS -o "${BUILD_DIR}/${NAME}" $objs $LFLAGS;;
		a)
			STEP ar cr ${BUILD_DIR}/${NAME}.a $objs;;
		o)
			STEP $CXX $FLAGS -o "${BUILD_DIR}/${NAME}.o" $objs -r;;
		so)
			STEP $CXX $FLAGS -o "${BUILD_DIR}/${NAME}.so" $objs -shared -fPIC;;
	esac
}

recursively () {
	local cmd="$1"
	shift
	local dir="$1"
	shift

	(
		[[ -f "$dir"/opts.sh ]] && . "$dir"/opts.sh
		for file in "$dir"/*; do if [[ -f "$file" ]]; then
			"$cmd" "$file"
		fi; done
		for subdir in "$dir"/*; do if [[ -d "$subdir" ]]; then
			recursively "$cmd" "$subdir"
		fi; done
	)
}
