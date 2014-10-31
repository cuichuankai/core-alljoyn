/**
 * @file
 * NamedPipeClientTransport is the transport mechanism between a client and the router
 * over named pipe.
 */

/******************************************************************************
 * Copyright (c) 2014, AllSeen Alliance. All rights reserved.
 *
 *    Permission to use, copy, modify, and/or distribute this software for any
 *    purpose with or without fee is hereby granted, provided that the above
 *    copyright notice and this permission notice appear in all copies.
 *
 *    THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 *    WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 *    MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 *    ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 *    WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 *    ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 *    OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 ******************************************************************************/

#ifndef _ALLJOYN_NAMEDPIPE_CLIENT_TRANSPORT_H
#define _ALLJOYN_NAMEDPIPE_CLIENT_TRANSPORT_H

#ifndef __cplusplus
#error Only include NamedPipeClientTransport.h in C++ code.
#endif

#include <Status.h>

#include <qcc/platform.h>
#include <qcc/String.h>

#include "Transport.h"
#include "RemoteEndpoint.h"

namespace ajn {

/**
 * @brief A class for NamedPipe Client Transports used in AllJoyn Windows clients and services.
 */
class NamedPipeClientTransport : public Transport, public _RemoteEndpoint::EndpointListener {

  public:
    /**
     * Create a Client based transport for use by clients and services.
     *
     * @param bus The BusAttachment associated with this endpoint
     */
    NamedPipeClientTransport(BusAttachment& bus);

    /**
     * Destructor
     */
    virtual ~NamedPipeClientTransport();

    /**
     * Start the transport and associate it with a router.
     *
     * @return
     *      - ER_OK if successful.
     *      - an error status otherwise.
     */
    QStatus Start();

    /**
     * Stop the transport.
     *
     * @return
     *      - ER_OK if successful.
     *      - an error status otherwise.
     */
    QStatus Stop();

    /**
     * Pend the caller until the transport stops.
     *
     * @return
     *      - ER_OK if successful
     *      - an error status otherwise.
     */
    QStatus Join();

    /**
     * Determine if this transport is running. Running means Start() has been called.
     *
     * @return  Returns true if the transport is running.
     */
    bool IsRunning() { return m_running; }

    /**
     * Get the transport mask for this transport
     *
     * @return the TransportMask for this transport.
     */
    TransportMask GetTransportMask() const { return TRANSPORT_LOCAL; }

    /**
     * Normalize a transport specification.
     * Given a transport specification, convert it into a form which is guaranteed to have a one-to-one
     * relationship with a transport.
     *
     * @param inSpec    Input transport connect spec.
     * @param outSpec   Output transport connect spec.
     * @param argMap    Parsed parameter map.
     *
     * @return ER_OK if successful.
     */
    QStatus NormalizeTransportSpec(const char* inSpec, qcc::String& outSpec, std::map<qcc::String, qcc::String>& argMap) const { return ER_NOT_IMPLEMENTED; }

    /**
     * Validates the connect spec for Named Pipe Transport
     *
     * @param connectSpec    Input transport connect spec.
     *
     * @return ER_OK if successful.
     */
    QStatus IsConnectSpecValid(const char* connectSpec);

    /**
     * Connect to a specified remote AllJoyn/DBus address.
     *
     * @param connectSpec    Transport specific key/value args used to configure the client-side endpoint.
     *                       The form of this string is @c "<transport>:<key1>=<val1>,<key2>=<val2>..."
     * @param opts           Requested sessions opts.
     * @param newep          [OUT] Endpoint created as a result of successful connect.
     * @return
     *      - ER_OK if successful.
     *      - an error status otherwise.
     */
    QStatus Connect(const char* connectSpec, const SessionOpts& opts, BusEndpoint& newep);

    /**
     * Disconnect from a specified AllJoyn/DBus address.
     *
     * @param connectSpec    The connectSpec used in Connect.
     *
     * @return
     *      - ER_OK if successful.
     *      - an error status otherwise.
     */
    QStatus Disconnect(const char* connectSpec);

    /**
     * Set a listener for transport related events.  There can only be one
     * listener set at a time. Setting a listener implicitly removes any
     * previously set listener.
     *
     * @param listener  Listener for transport related events.
     */
    void SetListener(TransportListener* listener) { m_listener = listener; }

    /**
     * Returns the name of this transport
     */
    const char* GetTransportName() const { return TransportName; }

    /**
     * Indicates whether this transport is used for client-to-bus or bus-to-bus connections.
     *
     * @return  Always returns false, ClientTransports are only used to connect to a local daemon.
     */
    bool IsBusToBus() const { return false; }

    /**
     * Name of transport used in transport specs.
     */
    static const char* TransportName;

    /**
     * Returns true if a client transport is available on this platform. Some platforms only support
     * a bundled daemon so don't have a client transport. Transports must have names so if the
     * transport has no name it is not available.
     */
    static bool IsAvailable() { return TransportName != NULL; }

    /**
     * Callback for ClientEndpoint exit.
     *
     * @param endpoint   ClientEndpoint instance that has exited.
     */
    void EndpointExit(RemoteEndpoint& endpoint);

  private:
    BusAttachment& m_bus;               /**< The message bus for this transport */
    bool m_running;                     /**< True after Start() has been called, before Stop() */
    TransportListener* m_listener;      /**< Registered TransportListener */
    RemoteEndpoint m_endpoint;          /**< The active endpoint */
};

} // namespace ajn

#endif // _ALLJOYN_NAMEDPIPE_CLIENT_TRANSPORT_H
