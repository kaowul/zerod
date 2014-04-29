#ifndef SESSION_H
#define SESSION_H

#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>
#include <uthash/uthash.h>

struct znat;
struct zclient;

/**
 * Session.
 */
struct zsession {
    // session ip (host order)
    uint32_t ip;
    // assigned client for this session
    struct zclient *client;

    // last activity time (atomic)
    uint64_t last_activity;
    // last authentication time  (atomic)
    uint64_t last_auth;
    // last accounting update time  (atomic)
    uint64_t last_acct;

    // accounting alive flag
    bool accounting_alive;
    // delete flag (for overlord)
    bool delete_flag;

    // packet counters  (atomic)
    uint32_t packets_up;
    uint32_t packets_down;
    // traffic counters in bytes (atomic)
    uint64_t traff_up;
    uint64_t traff_down;

    // reference count (atomic)
    uint32_t refcnt;

    // last NAT cleanup time
    uint64_t last_nat_cleanup;
    // lock for atomic NAT allocating
    pthread_spinlock_t _nat_lock;
    // NAT handle
    struct znat *nat;

    // rwlock
    pthread_rwlock_t lock_client;
    // hash handle (lookup by ip)
    UT_hash_handle hh;
};

struct zsession *session_acquire(uint32_t ip, bool existing_only);
void session_release(struct zsession *sess);
void session_remove(struct zsession *sess);
void session_destroy(struct zsession *sess);
struct znat *session_get_nat(struct zsession *sess, bool allocate);

#endif // SESSION_H