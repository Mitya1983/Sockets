#include "socket_error.hpp"
#include <map>

struct SocketErrorCategory : std::error_category {
    [[nodiscard]] const char* name() const noexcept override;

    [[nodiscard]] std::string message(int ec) const override;
};

inline const SocketErrorCategory g_socket_error_category;

inline const std::map< tristan::sockets::Error, const char* > g_socket_code_descriptions{
    {tristan::sockets::Error::SUCCESS,                                   "Success"                                                                                                  },
    {tristan::sockets::Error::SOCKET_PROTOCOL_NOT_SUPPORTED,             "The protocol type or the specified protocol is not supported within this communication domain"            },
    {tristan::sockets::Error::SOCKET_PROCESS_TABLE_IS_FULL,              "The per-process descriptor table is full"                                                                 },
    {tristan::sockets::Error::SOCKET_SYSTEM_TABLE_IS_FULL,               "The system file table is full"                                                                            },
    {tristan::sockets::Error::SOCKET_NOT_ENOUGH_PERMISSIONS,             "Permission to create a socket of the specified type and/or protocol is denied"                            },
    {tristan::sockets::Error::SOCKET_NOT_ENOUGH_MEMORY,
     "Insufficient buffer space is available. The socket cannot be created until sufficient resources are freed"                                                                    },
    {tristan::sockets::Error::SOCKET_WRONG_PROTOCOL,                     "The protocol is the wrong type for the socket"                                                            },
    {tristan::sockets::Error::SOCKET_WRONG_IP_FORMAT,                    "Wrong ip format"                                                                                          },
    {tristan::sockets::Error::SOCKET_NOT_INITIALISED,                    "Socket is not initialised"                                                                                },
    {tristan::sockets::Error::SOCKET_FCNTL_ERROR,                        "Fcntl failed to change O_NONBLOCKING flag"                                                                },
    {tristan::sockets::Error::SOCKET_NOT_CONNECTED,                      "Socket is not connected"                                                                                  },
    {tristan::sockets::Error::SOCKET_TIMED_OUT,                          "Socket operation timed out"                                                                               },
    {tristan::sockets::Error::BIND_NOT_ENOUGH_PERMISSIONS,               "The address is protected, and the user is not the superuser"                                              },
    {tristan::sockets::Error::BIND_ADDRESS_IN_USE,                       "The given address is already in use"                                                                      },
    {tristan::sockets::Error::BIND_BAD_FILE_DESCRIPTOR,                  "Socket is not a valid file descriptor"                                                                    },
    {tristan::sockets::Error::BIND_ALREADY_BOUND,                        "The socket is already bound to an address"                                                                },
    {tristan::sockets::Error::BIND_FILE_DESCRIPTOR_IS_NOT_SOCKET,        "The file descriptor does not refer to a socket"                                                           },
    {tristan::sockets::Error::BIND_ADDRESS_NOT_AVAILABLE,                "A nonexistent interface was requested or the requested address was not local"                             },
    {tristan::sockets::Error::BIND_ADDRESS_OUTSIDE_USER_SPACE,           "Address points outside the user's accessible address space"                                               },
    {tristan::sockets::Error::BIND_TO_MANY_SYMBOLIC_LINKS,               "Too many symbolic links were encountered in resolving address"                                            },
    {tristan::sockets::Error::BIND_NAME_TO_LONG,                         "Address is to long"                                                                                       },
    {tristan::sockets::Error::BIND_NO_ENTRY,                             "A component in the directory prefix of the socket pathname does not exists"                               },
    {tristan::sockets::Error::BIND_NO_MEMORY,                            "Insufficient kernel memory was available"                                                                 },
    {tristan::sockets::Error::BIND_NOT_DIRECTORY,                        "A component of the path prefix is not a directory"                                                        },
    {tristan::sockets::Error::BIND_READ_ONLY_FS,                         "The socket inode would reside on a read-only filesystem"                                                  },
    {tristan::sockets::Error::LISTEN_ADDRESS_IN_USE,                     "Another socket is already listening on the same port"                                                     },
    {tristan::sockets::Error::LISTEN_BAD_FILE_DESCRIPTOR,                "Socket is not a valid file descriptor"                                                                    },
    {tristan::sockets::Error::LISTEN_FILE_DESCRIPTOR_IS_NOT_SOCKET,      "The file descriptor does not refer to a socket"                                                           },
    {tristan::sockets::Error::LISTEN_PROTOCOL_NOT_SUPPORTED,             "The socket is not of a type that supports the listen() operation"                                         },
    {tristan::sockets::Error::LISTEN_ALREADY_CONNECTED,                  "The socket is is already connected"                                                                       },
    {tristan::sockets::Error::LISTEN_NOT_BOUND,                          "The socket is not bind to any address"                                                                    },
    {tristan::sockets::Error::ACCEPT_SOCKET_IS_NOT_IN_LISTEN_MODE,       "The socket is not in listen mode"                                                                         },
    {tristan::sockets::Error::ACCEPT_ALREADY_CONNECTED,                  "The socket is is already connected"                                                                       },
    {tristan::sockets::Error::ACCEPT_NOT_BOUND,                          "The socket is not bind to any address"                                                                    },
    {tristan::sockets::Error::ACCEPT_TRY_AGAIN,                          "The socket is marked nonblocking and no connections are present to be accepted"                           },
    {tristan::sockets::Error::ACCEPT_BAD_FILE_DESCRIPTOR,                "The socket is not an open file descriptor"                                                                },
    {tristan::sockets::Error::ACCEPT_CONNECTION_ABORTED,                 "A connection has been aborted"                                                                            },
    {tristan::sockets::Error::ACCEPT_ADDRESS_OUTSIDE_USER_SPACE,         "The address argument is not in a writable part of the user address space"                                 },
    {tristan::sockets::Error::ACCEPT_INTERRUPTED,                        "The system call was interrupted by a signal that was caught before a valid connection arrived"            },
    {tristan::sockets::Error::ACCEPT_INVALID_VALUE,                      "Socket is not listening for connections, or address length is invalid"                                    },
    {tristan::sockets::Error::ACCEPT_PER_PROCESS_LIMIT_REACHED,          "The per-process limit on the number of open file descriptors has been reached"                            },
    {tristan::sockets::Error::ACCEPT_SYSTEM_WIDE_LIMIT_REACHED,          "The system-wide limit on the total number of open files has been reached"                                 },
    {tristan::sockets::Error::ACCEPT_NOT_ENOUPH_MEMORY,
     "Not enough free memory. This often means that the memory allocation is limited by the socket buffer limits, not by the system memory"                                         },
    {tristan::sockets::Error::ACCEPT_FILE_DESCRIPTOR_IS_NOT_SOCKET,      "The socket file descriptor does not refer to a socket"                                                    },
    {tristan::sockets::Error::ACCEPT_OPTION_IS_NOT_SUPPORTED,            "The referenced socket is not of type SOCK_STREAM"                                                         },
    {tristan::sockets::Error::ACCEPT_FIREWALL,                           "Firewall rules forbid connection"                                                                         },
    {tristan::sockets::Error::ACCEPT_PROTOCOL_ERROR,                     "Protocol error"                                                                                           },
    {tristan::sockets::Error::CONNECT_SOCKET_IS_IN_LISTEN_MODE,          "The socket is is listen mode"                                                                             },
    {tristan::sockets::Error::CONNECT_NOT_ENOUGH_PERMISSIONS,            "Local address is already in use"                                                                          },
    {tristan::sockets::Error::CONNECT_ADDRESS_IN_USE,
     "For UNIX domain sockets, which are identified by pathname: Write permission is denied on the socket file, or search permission "
     "is denied for one of the directories in the path prefix. The user tried to connect to a broadcast address without having the "
     "socket broadcast flag enabled or the connection request failed because of a local firewall rule"                                                                              },
    {tristan::sockets::Error::CONNECT_ADDRESS_NOT_AVAILABLE,
     "The socket referred to had not previously been bound to an address and, upon attempting to bind it to an ephemeral port"                                                      },
    {tristan::sockets::Error::CONNECT_AF_NOT_SUPPORTED,                  "The passed address didn't have the correct address family in its sa_family field"                         },
    {tristan::sockets::Error::CONNECT_TRY_AGAIN,
     "For nonblocking UNIX domain sockets, the socket is nonblocking, and the connection cannot be completed immediately.  For other "
     "socket families, there are insufficient entries in the routing cache"                                                                                                         },
    {tristan::sockets::Error::CONNECT_ALREADY_IN_PROCESS,                "The socket is nonblocking and a previous connection attempt has not yet been completed"                   },
    {tristan::sockets::Error::CONNECT_BAD_FILE_DESCRIPTOR,               "Socket is not a valid open file descriptor"                                                               },
    {tristan::sockets::Error::CONNECT_CONNECTION_REFUSED,                "On a stream socket found no one listening on the remote address"                                          },
    {tristan::sockets::Error::CONNECT_ADDRESS_OUTSIDE_USER_SPACE,        "The socket structure address is outside the user's address space"                                         },
    {tristan::sockets::Error::CONNECT_IN_PROGRESS,                       "The socket is nonblocking and the connection cannot be completed immediately"                             },
    {tristan::sockets::Error::CONNECT_INTERRUPTED,                       "The system call was interrupted by a signal that was caught"                                              },
    {tristan::sockets::Error::CONNECT_CONNECTED,                         "The socket is already connected"                                                                          },
    {tristan::sockets::Error::CONNECT_NETWORK_UNREACHABLE,               "Network is unreachable"                                                                                   },
    {tristan::sockets::Error::CONNECT_FILE_DESCRIPTOR_IS_NOT_SOCKET,     "The file descriptor does not refer to a socket"                                                           },
    {tristan::sockets::Error::CONNECT_PROTOCOL_NOT_SUPPORTED,            "The protocol type or the specified protocol is not supported within this communication domain"            },
    {tristan::sockets::Error::SSL_METHOD_ERROR,                          "TLS_client_method() returned nullptr"                                                                     },
    {tristan::sockets::Error::SSL_CONTEXT_ERROR,                         "SSL_CTX_new returned nullptr"                                                                             },
    {tristan::sockets::Error::SSL_INIT_ERROR,                            "SSL_new returned nullptr"                                                                                 },
    {tristan::sockets::Error::SSL_TRY_AGAIN,                             "Underlying BIO is nonblocking and operation should be performed once more"                                },
    {tristan::sockets::Error::SSL_CONNECT_ERROR,                         "SSL_connect returned an error: "                                                                          },
    {tristan::sockets::Error::SSL_CERTIFICATE_ERROR,                     "SSL_get_peer_certificate returned nullptr"                                                                },
    {tristan::sockets::Error::SSL_CERTIFICATE_VERIFICATION_HOST,         "Host was not verified"                                                                                    },
    {tristan::sockets::Error::SSL_CERTIFICATE_VERIFICATION_START_DATE,   "Certificate start date is in future"                                                                      },
    {tristan::sockets::Error::SSL_CERTIFICATE_VERIFICATION_END_DATE,     "Certificate end date is in the past"                                                                      },
    {tristan::sockets::Error::SSL_CERTIFICATE_VALIDATION_FAILED,         "Certificate had not passed the validation"                                                                },
    {tristan::sockets::Error::SSL_CLOSED_BY_PEER,                        "Connection was closed by host"                                                                            },
    {tristan::sockets::Error::SSL_IO_ERROR,                              "Some non-recoverable, fatal I/O error occurred"                                                           },
    {tristan::sockets::Error::SSL_FATAL_ERROR,                           "A non-recoverable, fatal error in the SSL library occurred, usually a protocol error"                     },
    {tristan::sockets::Error::SSL_UNKNOWN_ERROR,                         "Unknown error"                                                                                            },
    {tristan::sockets::Error::WRITE_TRY_AGAIN,                           "The socket is marked nonblocking and the requested operation would block"                                 },
    {tristan::sockets::Error::WRITE_BAD_FILE_DESCRIPTOR,                 "Socket is not a valid open file descriptor"                                                               },
    {tristan::sockets::Error::WRITE_DESTINATION_ADDRESS,                 "The socket refers to a datagram socket for which a peer address has not been set using connect"           },
    {tristan::sockets::Error::WRITE_USER_QUOTA,                          "he user's quota of disk blocks on the filesystem containing the file referred to by fd has been exhausted"},
    {tristan::sockets::Error::WRITE_INTERRUPTED,                         "A signal occurred before any data was transmitted"                                                        },
    {tristan::sockets::Error::WRITE_BUFFER_OUT_OF_RANGE,                 "Buffer is outside your accessible address space"                                                          },
    {tristan::sockets::Error::WRITE_BIG,
     "An attempt was made to write a file that exceeds the implementation-defined maximum file size or the process's file size limit, or to write at a "
     "position past the maximum allowed offset"                                                                                                                                     },
    {tristan::sockets::Error::WRITE_INVALID_ARGUMENT,                    "Invalid argument passed"                                                                                  },
    {tristan::sockets::Error::WRITE_LOW_LEVEL_IO,                        "A low-level I/O error occurred while modifying the inode"                                                 },
    {tristan::sockets::Error::WRITE_NO_SPACE,                            "The device containing the file referred to by fd has no room for the data"                                },
    {tristan::sockets::Error::WRITE_NOT_PERMITTED,                       "The operation was prevented by a file seal"                                                               },
    {tristan::sockets::Error::WRITE_PIPE,                                "The socket is connected to a pipe or socket whose reading end is closed"                                  },
    {tristan::sockets::Error::READ_TRY_AGAIN,
     "The file descriptor fd refers to a file other than a socket and has been marked nonblocking, and the read would block"                                                        },
    {tristan::sockets::Error::READ_BAD_FILE_DESCRIPTOR,                  "The socket is not a valid file descriptor or is not open for reading"                                     },
    {tristan::sockets::Error::READ_BUFFER_OUT_OF_RANGE,                  "Buffer is outside your accessible address space"                                                          },
    {tristan::sockets::Error::READ_INTERRUPTED,                          "A signal occurred before any data was read"                                                               },
    {tristan::sockets::Error::READ_INVALID_FILE_DESCRIPTOR,              "The socket is attached to an which is unsuitable for reading"                                             },
    {tristan::sockets::Error::READ_IO,                                   "I/O error"                                                                                                },
    {tristan::sockets::Error::READ_IS_DIRECTORY,                         "File descriptor refers to a directory"                                                                    },
    {tristan::sockets::Error::READ_EOF,                                  "EOF received"                                                                                             },
    {tristan::sockets::Error::READ_DONE,                                 "Read until finished reading by reaching the delimiter"                                                    },
    {tristan::sockets::Error::SHUTDOWN_INVALID_SOCKET_ARGUMENT,          "The socket argument is not a valid file descriptor"                                                       },
    {tristan::sockets::Error::SHUTDOWN_INVALID_SHUTDOWN_OPTION_PROVIDED, "The how argument is invalid"                                                                              },
    {tristan::sockets::Error::SHUTDOWN_NOT_CONNECTED,                    "The socket is not connected"                                                                              },
    {tristan::sockets::Error::SHUTDOWN_INVALID_FILE_DESCRIPTOR,          "The socket argument does not refer to a socket"                                                           },
    {tristan::sockets::Error::SHUTDOWN_NOT_ENOUPH_MEMORY,                "Insufficient resources were available in the system to perform the operation"                             },
};

auto tristan::sockets::makeError(tristan::sockets::Error error_code) -> std::error_code { return {static_cast< int >(error_code), g_socket_error_category}; }

auto SocketErrorCategory::name() const noexcept -> const char* { return "SocketCategory"; }

auto SocketErrorCategory::message(int ec) const -> std::string { return {g_socket_code_descriptions.at(static_cast< tristan::sockets::Error >(ec))}; }