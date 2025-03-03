#!/bin/sh

godot_bin=""

while [[ $# -gt 0 ]]; do
    case "$1" in
        --godot_bin)
            godot_bin="$2"
            shift # past argument
            shift # past value
            ;;
        *)
            # unknown option or argument
            break
            ;;
    esac
done

if [ -z "$godot_bin" ]; then
    echo "'--godot_bin' was not provided."
    echo "Please use '--godot_bin /path/to/godot' to specify the Godot binary."
    exit 1
fi

"$godot_bin" --path . -s -d res://addons/gdUnit4/bin/GdUnitCmdTool.gd "$@"
exit_code=$?
echo "Run tests ends with $exit_code"

"$godot_bin" --headless --path . --quiet -s -d res://addons/gdUnit4/bin/GdUnitCopyLog.gd "$@" > /dev/null
exit_code2=$?
exit $exit_code