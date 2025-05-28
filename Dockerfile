# Stage 1: Build stage
FROM ubuntu:latest AS build

# Install required packages
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    && rm -rf /var/lib/apt/lists/*

# Set the working directory
WORKDIR /app

# Copy the source code into the container
COPY /task_scheduler .

# Build the application
RUN mkdir build && cd build && \
    cmake .. && \
    make

# Stage 2: Runtime stage
FROM ubuntu:latest

# Install runtime dependencies if needed
RUN apt-get update && apt-get install -y \
    libstdc++6 \
    && rm -rf /var/lib/apt/lists/*

# Copy the executable from the build stage
COPY --from=build /app/build/task_scheduler /usr/local/bin/task_scheduler

# Command to run the binary
CMD ["task_scheduler"]