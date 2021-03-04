
#include <signal.h>
#include <string.h>

#include <iostream>

#include "include/EventLoop.h"
#include "network/TCPServer.h"
#include "network/TCPClient.h"
#include "network/IConnection.h"
#include "util/Buffer.h"

int main()
{
    signal(SIGPIPE, SIG_IGN);
    signal(SIGHUP, SIG_IGN);

    auto loop = std::make_shared<EventLoop>();
    loop->Init();

    TCPServer gate(loop, "192.168.0.1:10240");

    // DataServer/GameServer
    TCPClient db(loop, "192.168.0.1:10241");
    TCPClient game(loop, "192.168.0.1:10242");


    // TODO use Protocol class to simplify code

    db.SetMessageCallBack([&](ConnSPtr conn) -> int
                          {
                              auto connBuf = conn->GetBuffer();
                              if (connBuf->GetLength() >= 13)
                              {
                                  auto taglen = connBuf->PeekTagLen();
                                  if (taglen.second <= connBuf->GetLength() - 1)
                                  {
                                      char tag = connBuf->ReadInt8();

                                      // connect success
                                      if (tag == 's')
                                      {
                                          connBuf->ReadInt32();
                                          auto serial_id = connBuf->ReadInt64();
                                          auto user_id = connBuf->ReadInt64();

                                          auto c = gate.GetConnection(serial_id);
                                          if (c != nullptr)
                                          {
                                              c->SetAuthorized(true);
                                              c->setUserId(user_id);
                                          }
                                      }
                                  }
                                  else
                                  {
                                      return READ_INCOMPLETE;
                                  }
                              }
                              else
                              {
                                  return READ_INCOMPLETE;
                              }
                              return READ_SUCCESS;
                          });

    game.SetMessageCallBack([&](ConnSPtr conn) -> int
                            {

                            });

    db.Connect();
    game.Connect();


    gate.SetConnectionCallBack([](ConnSPtr conn) -> int
                               {
                                   // check username and password
                                   conn->SetAuthorized(false);
                                   auto buf = std::make_shared<Buffer>();

                                   // need Auth
                                   buf->AppendInt8('A');
                                   buf->AppendInt32(4);
                                   conn->Send(buf);

                                   return READ_SUCCESS;
                               });

    gate.SetMessageCallBack([&](ConnSPtr conn) -> int
                            {
                                auto connBuf = conn->GetBuffer();
                                if (conn->IsAuthorized())
                                {
                                    if (connBuf->GetLength() >= 5)
                                    {
                                        auto taglen = connBuf->PeekTagLen();
                                        if (taglen.second <= connBuf->GetLength() - 1)
                                        {
                                            char tag = connBuf->ReadInt8();
                                            if (tag == 'l')
                                            {
                                                int len = connBuf->ReadInt32();
                                                std::string value = connBuf->ReadString(len);

                                                auto sendBuf = std::make_shared<Buffer>();
                                                sendBuf->AppendInt8('l');
                                                sendBuf->AppendInt32(len + 8);
                                                sendBuf->AppendInt64(conn->GetSerialId());
                                                sendBuf->AppendString(value);
                                                db.Send(sendBuf);
                                            }
                                            else
                                            {
                                                connBuf->Clear();
                                                // TODO may need close the connection.
                                                return READ_TYPE_ERROR;
                                            }
                                        }
                                        else
                                        {
                                            return READ_INCOMPLETE;
                                        }
                                    }
                                    else
                                    {
                                        return READ_INCOMPLETE;
                                    }
                                }
                                else
                                {
                                    if (connBuf->GetLength() >= 5)
                                    {
                                        // TODO send to GameServer
                                    }
                                    else
                                    {
                                        return READ_INCOMPLETE;
                                    }
                                }
                            });
    gate.SetCloseCallBack([&](ConnSPtr conn) -> int
                          {

                          });

    gate.Start();

    loop->Run();

    return 0;
}
