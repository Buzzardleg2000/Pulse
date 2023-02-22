#!/bin/bash
# Call me from a directory that contains both the pulse source directory and your build directory

# Usage message
USAGE=$(cat <<-END

Usage: dockcross [container-name] [pulse-source-directory] [build-directory] [existing build directory]


Example Usage: dockcross linux-x64-clang ./Pulse/engine ./builds

A new directory will be created under the provided build directory to build in
The directory name will be 'pulse-engine-' and the container name.
For example: pulse-engine-linux-x64-clang

Options:
  container-name        This is the name of the container to build in
                        https://hub.docker.com/u/dockcross
                        Only provide the string after 'dockcross/'
                        example : linux-x64-clang

  pulse-soure-directory The relative directory of the pulse source code

  build-directory       The relative directory to build pulse
END
)

if [ "$#" -lt 3 ]
then
  echo "$USAGE"
  exit 1
fi

container=$1
echo "Using container: $container"

pulseDir=$2
if [ -d "$pulseDir" ]
then
    echo "Using Pulse source code directory: $pulseDir"
else
    echo "$USAGE"
    exit 1
fi

rootBldDir=$3
bldDir="$rootBldDir/pulse-engine-$container"
if [ -d "$rootBldDir" ]
then
    echo "Building pulse in: $pulseDir"
    if [ -d "$bldDir" ]
    then
      echo "Deleting existing $bldDir directory..."
      rm -r "$bldDir"
    fi
    echo "Creating build directory: $bldDir"
    mkdir "$bldDir"
else
    echo "$USAGE"
    exit 1
fi

# If we are building android need binding files from another build
if [[ "$container" == *android* ]]
then
  if [ "$#" -lt 4 ]
  then
    echo "!! To build for Android, you must provide an existing build directory with the autogenerated protobuf files!!"
    echo "The android protoc compiler cannot run on our linux based docker host"
    echo "$USAGE"
    exit 1
  fi

  echo "Copying pregenerated protobuf files from $4 to $bldDir"
  if [ ! -d "$bldDir/Innerbuild/src/cpp/pulse" ]
  then
    mkdir -p "$bldDir/Innerbuild/src/cpp/pulse"
  fi
  cp -r "$4/Innerbuild/src/cpp/pulse" "$bldDir/Innerbuild/src/cpp"
  # Get touch file so we don't run protoc
  cp "$4/Innerbuild/src/schema_last_built" "$bldDir/Innerbuild/src/"
fi

# Run this command from a directory that contains both your source and build directories
docker run --rm dockcross/$container > "./$container"
# This create a file we will use to invoke our container
# Next set execution permissions
chmod +x "./$container"
# Run CMake
# -B is the relative path to my build directory
# -H is the relative path to my Pulse source directory
./$container cmake -DPulse_JAVA_API:BOOL=OFF -B$bldDir -H$pulseDir -GNinja
# Use ninja to build (provided in the docker)
./$container ninja -C$bldDir
# The PulseC.so for the target platform will be in the $bldDir/install/bin directory
