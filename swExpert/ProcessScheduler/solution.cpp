#define MAXPROC 50001
#define MAXPRIO 102
#define END_PROCESS 1
#define END_QUANTUM 2
#define TOP_PRIO 3

class HeapNode {
public:
    int time;
    int idx;
    int priority;
    int event;
};

class Heap {
public:
    HeapNode arr[MAXPROC * 2];
    int length;

    void init() { length = 0; }

    bool compare(int parent, int child) {
        if (arr[parent].time > arr[child].time) { return true; }
        if (arr[parent].time == arr[child].time && arr[parent].event > arr[child].event) { return true; }
        return false;
    }

    void push(int time, int id, int prio, int e) {
        HeapNode last;
        last.time = time; last.idx = id; last.priority = prio; last.event = e;

        int idx = length;
        arr[length++] = last;

        while ((idx - 1) / 2 >= 0 && compare((idx - 1) / 2, idx)) {
            HeapNode temp = arr[idx];
            arr[idx] = arr[(idx - 1) / 2];
            arr[(idx - 1) / 2] = temp;
            idx = (idx - 1) / 2;
        }
    }

    HeapNode pop() {
        HeapNode ans = arr[0];
        arr[0] = arr[--length];

        int idx = 0;
        int left, right, child;

        while (2 * idx + 1 < length) {
            left = 2 * idx + 1;
            right = 2 * idx + 2;

            if (right < length) {
                if (compare(left, right)) { child = right; }
                else { child = left; }
            }
            else { child = left; }

            if (compare(idx, child)) {
                HeapNode temp = arr[idx];
                arr[idx] = arr[child];
                arr[child] = temp;
                idx = child;
            }
            else { break; }
        }
        return ans;
    }
};

class Queue {
public:
    int arr[MAXPROC];
    int st, ed, length;

    void init() {
        st = 0;
        ed = 0;
        length = 0;
    }

    void push(int a) {
        length++;
        arr[ed++] = a;
    }

    int pop() {
        length--;
        return arr[st++];
    }

    int front() {
        return arr[st];
    }
};

class Process {
public:
    int priority;
    int remain;
};

class ProcessingUnit {
public:
    int processing;
    int time_start;
    int priority;

    void init() {
        processing = 0;
        time_start = 0;
        priority = 0;
    }

    void assign(int proc, int t, int p) {
        processing = proc;
        time_start = t;
        priority = p;
    }
};

int min(int a, int b) {
    if (a < b) { return a; }
    return b;
}

int totalQueue;
int unitQuantum;
Queue queue[MAXPRIO];
Process process[MAXPROC];
Heap pQueue;
ProcessingUnit cpu;

int k, nComplete;
int completed[MAXPROC];

int topPrio() {
    for (int i = MAXPRIO - 1; i > 0; i--) {
        while (queue[i].length > 0 && (process[queue[i].front()].priority != i || process[queue[i].front()].remain == 0)) {
            queue[i].pop();
        }

        if (queue[i].length > 0) {
            return queue[i].front();
        }
    }
    return 0;
}

void startProcess(int idx, int time, int priority) {
    if (idx == 0) { cpu.init(); }

    cpu.assign(idx, time, priority);
    if (unitQuantum < process[idx].remain) {
        pQueue.push(time + unitQuantum, idx, priority, END_QUANTUM);
    }
    else {
        pQueue.push(time + process[idx].remain, idx, priority, END_PROCESS);
    }
}

void updateTime(int tStamp) {
    while (pQueue.length > 0 && pQueue.arr[0].time <= tStamp) {
        HeapNode curr = pQueue.pop();

        if (cpu.processing == 0) { continue; }
        if (process[curr.idx].remain == 0) { continue; }

        if (curr.event == END_PROCESS) {
            if (cpu.processing != curr.idx) { continue; }
            if (cpu.priority != curr.priority) { continue; }
            if (curr.time - cpu.time_start != process[curr.idx].remain) { continue; }

            process[curr.idx].remain -= curr.time - cpu.time_start;
            queue[cpu.priority].pop();
            totalQueue--;

            completed[nComplete++] = curr.idx;

            int idx = topPrio();
            startProcess(idx, curr.time, process[idx].priority);
        }

        else if (curr.event == END_QUANTUM) {
            if (cpu.processing != curr.idx) { continue; }
            if (cpu.priority != curr.priority) { continue; }

            process[curr.idx].remain -= curr.time - cpu.time_start;
            queue[cpu.priority].push(queue[cpu.priority].pop());

            int idx = topPrio();
            startProcess(idx, curr.time, process[idx].priority);
        }

        else if (curr.event == TOP_PRIO) {
            if (cpu.time_start < curr.time && cpu.priority < MAXPRIO - 1) {
                queue[cpu.priority].push(queue[cpu.priority].pop());
            }

            process[cpu.processing].remain -= curr.time - cpu.time_start;
            cpu.time_start = curr.time;

            queue[MAXPRIO - 1].push(curr.idx);
            process[curr.idx].priority = MAXPRIO - 1;
            int idx = topPrio();
            if (curr.idx == idx) {
                startProcess(idx, curr.time, process[idx].priority);
            }
        }
    }

    if (cpu.time_start < tStamp) {
        process[cpu.processing].remain -= tStamp - cpu.time_start;
        cpu.time_start = tStamp;
    }
}

void init(int mQuantum) {
    pQueue.init();
    totalQueue = 0;
    cpu.init();
    unitQuantum = mQuantum;

    for (int i = 0; i < MAXPRIO; i++) {
        queue[i].init();
    }

    nComplete = 0; k = 0;
}

int addProcess(int tStamp, int pId, int mTime, int mPriority, int completedIds[]) {
    updateTime(tStamp);

    process[pId].remain = mTime;
    process[pId].priority = mPriority;
    totalQueue++;

    pQueue.push(tStamp + 1000, pId, mPriority, TOP_PRIO);
    queue[mPriority].push(pId);

    if (mPriority > cpu.priority) {
        if (cpu.priority > 0) {
            queue[cpu.priority].push(queue[cpu.priority].pop());
        }
        startProcess(pId, tStamp, mPriority);
    }

    if (nComplete > 0) {
        for (int i = k; i < nComplete; i++) {
            completedIds[i] = completed[i];
        }
        k = nComplete - 1;
    }

    return totalQueue;
}

int checkProcess(int tStamp, int completedIds[]) {
    updateTime(tStamp);

    if (nComplete > 0) {
        for (int i = k; i < nComplete; i++) {
            completedIds[i] = completed[i];
        }
        k = nComplete - 1;
    }

    return cpu.processing;
}