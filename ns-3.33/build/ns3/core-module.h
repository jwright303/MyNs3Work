
#ifdef NS3_MODULE_COMPILATION
# error "Do not include ns3 module aggregator headers from other modules; these are meant only for end user scripts."
#endif

#ifndef NS3_MODULE_CORE
    

// Module headers:
#include "abort.h"
#include "ascii-file.h"
#include "ascii-test.h"
#include "assert.h"
#include "attribute-accessor-helper.h"
#include "attribute-construction-list.h"
#include "attribute-container-accessor-helper.h"
#include "attribute-container.h"
#include "attribute-helper.h"
#include "attribute.h"
#include "boolean.h"
#include "breakpoint.h"
#include "build-profile.h"
#include "calendar-scheduler.h"
#include "callback.h"
#include "command-line.h"
#include "config.h"
#include "csv-reader.h"
#include "default-deleter.h"
#include "default-simulator-impl.h"
#include "deprecated.h"
#include "des-metrics.h"
#include "double.h"
#include "empty.h"
#include "enum.h"
#include "event-garbage-collector.h"
#include "event-id.h"
#include "event-impl.h"
#include "example-as-test.h"
#include "fatal-error.h"
#include "fatal-impl.h"
#include "global-value.h"
#include "hash-fnv.h"
#include "hash-function.h"
#include "hash-murmur3.h"
#include "hash.h"
#include "heap-scheduler.h"
#include "int-to-type.h"
#include "int64x64-128.h"
#include "int64x64-double.h"
#include "int64x64.h"
#include "integer.h"
#include "length.h"
#include "list-scheduler.h"
#include "log-macros-disabled.h"
#include "log-macros-enabled.h"
#include "log.h"
#include "make-event.h"
#include "map-scheduler.h"
#include "math.h"
#include "names.h"
#include "node-printer.h"
#include "non-copyable.h"
#include "nstime.h"
#include "object-base.h"
#include "object-factory.h"
#include "object-map.h"
#include "object-ptr-container.h"
#include "object-vector.h"
#include "object.h"
#include "pair.h"
#include "pointer.h"
#include "priority-queue-scheduler.h"
#include "ptr.h"
#include "random-variable-stream-helper.h"
#include "random-variable-stream.h"
#include "ref-count-base.h"
#include "rng-seed-manager.h"
#include "rng-stream.h"
#include "scheduler.h"
#include "show-progress.h"
#include "simple-ref-count.h"
#include "simulation-singleton.h"
#include "simulator-impl.h"
#include "simulator.h"
#include "singleton.h"
#include "string.h"
#include "synchronizer.h"
#include "system-condition.h"
#include "system-mutex.h"
#include "system-path.h"
#include "system-thread.h"
#include "system-wall-clock-ms.h"
#include "system-wall-clock-timestamp.h"
#include "test.h"
#include "time-printer.h"
#include "timer-impl.h"
#include "timer.h"
#include "trace-source-accessor.h"
#include "traced-callback.h"
#include "traced-value.h"
#include "type-id.h"
#include "type-name.h"
#include "type-traits.h"
#include "uinteger.h"
#include "unix-fd-reader.h"
#include "unused.h"
#include "valgrind.h"
#include "vector.h"
#include "watchdog.h"
#endif
