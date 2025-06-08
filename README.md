# TaskScheduler

A modern, multi-threaded task scheduler with CLI implementation using C++17. This project provides a robust solution for managing and executing prioritized tasks with different scheduling algorithms.

[![Build Status for branch: master](https://github.com/atahancaldir/TaskScheduler/actions/workflows/tests.yml/badge.svg)](https://github.com/atahancaldir/TaskScheduler/actions)
[![Build Status for branch: dev](https://github.com/atahancaldir/TaskScheduler/actions/workflows/tests.yml/badge.svg?branch=dev)](https://github.com/atahancaldir/TaskScheduler/actions)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

## 📋 Features

- Multiple scheduling algorithms:
  - First Come First Served (FCFS)
  - Round Robin
  - Priority-based scheduling
- Real-time task monitoring
- Command-line interface
- Task prioritization
- Multi-threaded execution
- Comprehensive logging system

## 🚀 Quick Start

### Prerequisites

- C++17 compatible compiler
- CMake (version 3.10 or higher)
- Make

### Installation

1. Clone the repository:

```bash
git clone https://github.com/atahancaldir/TaskScheduler.git
cd TaskScheduler
```

2. Build the project:

```bash
mkdir build && cd build
cmake ..
make
```

## 💻 Usage

### Command Line Options

```bash
./task_scheduler -h
Task scheduler command line interface
Usage:
  Task Scheduler [OPTION...]

  -a, --add              Add a new task
  -c, --clear            Clear the task queue
  -d, --delete arg       Delete a task by task ID
  -h, --help             Print usage
  -l, --list             List all tasks
  -m, --monitor          Monitor tasks in real-time
  -r, --run              Run/resume task scheduler
  -p, --pause            Pause task scheduler
      --log [=arg(=10)]  Show n lines of logs (default: 10)
  -s, --start arg        Start the task scheduler (default: fcfs)
  -x, --stop             Stop the task scheduler
```

### Examples

1. Start the scheduler with FCFS algorithm:

```bash
./task_scheduler -s fcfs
```

2. Add and monitor tasks:

```bash
>> ./task_scheduler -a  	   # Add a task
Command: sleep 10
Task is added to queue
>> ./task_scheduler -a 		   # Add a task
Command: python3 ~/Desktop/my_project.py
Task is added to queue
>> ./task_scheduler -r
Task queue is running
>> ./task_scheduler -m             # Monitor tasks in real-time
>> ./task_scheduler -x 		   # Stop scheduler
```

3. List current tasks:

```bash
./task_scheduler -l
```

Example output:

```bash
Scheduler type: First Come First Served
+----------------------------------------------------------------------------------------------------------------------------------+
| Order |                 Task ID                |                   Task Command                   |  Status  | Duration |   PID  |
+----------------------------------------------------------------------------------------------------------------------------------+
|   1   |  96768c38-afe2-42e8-44a5-431a5d898b6b  |                     sleep 10                     |  pending | 00:00:00 |  56159 |
|   2   |  be2a39a7-ee7f-4860-f558-9bcd7bfe6e38  |          python3 ~/Desktop/my_project.py         |  pending | 00:00:00 |  56768 |
+----------------------------------------------------------------------------------------------------------------------------------+
```

## 🤝 Contributing

Please feel free to submit a Pull Request. For major changes, please open an issue first to discuss what you would like to change.

### Development Setup

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add some amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## 📝 TODO

- [ ] Implement more scheduling algorithms:

  - [ ] Longest Job First (LJF)
  - [ ] Shortest Job First (SJF)
  - [ ] Longest Remaining Time First (LRTF)
- [ ] Add support for periodically repeating tasks

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
