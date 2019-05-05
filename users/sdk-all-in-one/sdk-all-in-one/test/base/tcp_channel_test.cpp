#define GTEST_USE_OWN_TR1_TUPLE 0
#include <gtest/gtest.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <fcntl.h>
#include <string.h>
#include <sys/epoll.h>



//#include "port_sock.h"
#include "ont_tcp_channel.h"
#include "ont/error.h"
#include "dynamic-stub-manager.h"
#include "tcp_channel_test.h"
using namespace std;

#define MAXEVENTS 64

MsgNode_t g_msg;
std::string g_ip = "127.0.0.1";
int g_port = 9054;
pthread_t g_tid;


//����:
//����:�����Ͱ�һ��TCP socket
//����:�˿�
//����ֵ:������socket
static int
create_and_bind (char *port)
{
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    int s, sfd;

    memset (&hints, 0, sizeof (struct addrinfo));
    hints.ai_family = AF_UNSPEC;     /* Return IPv4 and IPv6 choices */
    hints.ai_socktype = SOCK_STREAM; /* We want a TCP socket */
    hints.ai_flags = AI_PASSIVE;     /* All interfaces */

    s = getaddrinfo (NULL, port, &hints, &result);
    if (s != 0)
        {
            fprintf (stderr, "getaddrinfo: %s\n", gai_strerror (s));
            return -1;
        }

    for (rp = result; rp != NULL; rp = rp->ai_next)
        {
            sfd = socket (rp->ai_family, rp->ai_socktype, rp->ai_protocol);
            if (sfd == -1)
                continue;

            bool bReuseaddr = true;
            setsockopt( s, SOL_SOCKET, SO_REUSEADDR, ( const char* )&bReuseaddr, sizeof( bool) );

            s = bind (sfd, rp->ai_addr, rp->ai_addrlen);
            if (s == 0)
                {
                    /* We managed to bind successfully! */
                    break;
                }

            close (sfd);
        }

    if (rp == NULL)
        {
            fprintf (stderr, "Could not bind\n");
            return -1;
        }

    freeaddrinfo (result);

    return sfd;
}


//����
//����:����socketΪ��������
static int
make_socket_non_blocking (int sfd)
{
    int flags, s;

    //�õ��ļ�״̬��־
    flags = fcntl (sfd, F_GETFL, 0);
    if (flags == -1)
        {
            perror ("fcntl");
            return -1;
        }

    //�����ļ�״̬��־
    flags |= O_NONBLOCK;
    s = fcntl (sfd, F_SETFL, flags);
    if (s == -1)
        {
            perror ("fcntl");
            return -1;
        }

    return 0;
}


void* thread_echo_server(void* arg){
    int sfd, s;
    int efd;
    struct epoll_event event;
    struct epoll_event *events;
    int port = g_port;
    char port_str[10];
    snprintf(port_str, 10, "%d", port);


    sfd = create_and_bind (port_str);
    if (sfd == -1)
        abort ();

    s = make_socket_non_blocking (sfd);
    if (s == -1)
        abort ();

    s = listen (sfd, SOMAXCONN);
    if (s == -1)
        {
            perror ("listen");
            abort ();
        }

    //���˲���size��������,�˺�����epoll_create��ȫ��ͬ
    efd = epoll_create1 (0);
    if (efd == -1)
        {
            perror ("epoll_create");
            abort ();
        }

    event.data.fd = sfd;
    event.events = EPOLLIN | EPOLLET;//����,��Ե������ʽ
    //event.events = EPOLLIN;
    s = epoll_ctl (efd, EPOLL_CTL_ADD, sfd, &event);
    if (s == -1)
        {
            perror ("epoll_ctl");
            abort ();
        }

    /* Buffer where events are returned */
    events = (epoll_event*) calloc (MAXEVENTS, sizeof event);

    /* The event loop */
    while (1)
        {
            int n, i;

            n = epoll_wait (efd, events, MAXEVENTS, -1);

            for (i = 0; i < n; i++)
                {
                    if ((events[i].events & EPOLLERR) ||
                        (events[i].events & EPOLLHUP) ||
                        (!(events[i].events & EPOLLIN)))
                        {
                            /* An error has occured on this fd, or the socket is not
                               ready for reading (why were we notified then?) */
                            fprintf (stderr, "epoll error\n");
                            close (events[i].data.fd);
                            continue;
                        }

                    else if (sfd == events[i].data.fd)
                        {
                            /* We have a notification on the listening socket, which
                               means one or more incoming connections. */
                            while (1)
                                {
                                    struct sockaddr in_addr;
                                    socklen_t in_len;
                                    int infd;
                                    char hbuf[NI_MAXHOST], sbuf[NI_MAXSERV];

                                    in_len = sizeof in_addr;
                                    infd = accept (sfd, &in_addr, &in_len);
                                    if (infd == -1)
                                        {
                                            if ((errno == EAGAIN) ||
                                                (errno == EWOULDBLOCK))
                                                {
                                                    /* We have processed all incoming
                                                       connections. */
                                                    break;
                                                }
                                            else
                                                {
                                                    perror ("accept");
                                                    break;
                                                }
                                        }

                                    //����ַת��Ϊ���������߷�����
                                    s = getnameinfo (&in_addr, in_len,
                                                     hbuf, sizeof hbuf,
                                                     sbuf, sizeof sbuf,
                                                     NI_NUMERICHOST | NI_NUMERICSERV);//flag����:������������
                                    //������ַ�ͷ����ַ

                                    if (s == 0)
                                        {
                                            //printf("Accepted connection on descriptor %d " "(host=%s, port=%s)\n", infd, hbuf, sbuf);
                                        }

                                    /* Make the incoming socket non-blocking and add it to the
                                       list of fds to monitor. */
                                    s = make_socket_non_blocking (infd);
                                    if (s == -1)
                                        abort ();

                                    event.data.fd = infd;
                                    event.events = EPOLLIN | EPOLLET;
                                    //event.events = EPOLLIN;
                                    s = epoll_ctl (efd, EPOLL_CTL_ADD, infd, &event);
                                    if (s == -1)
                                        {
                                            perror ("epoll_ctl");
                                            abort ();
                                        }
                                }
                            continue;
                        }
                    else
                        {
                            /* We have data on the fd waiting to be read. Read and display it. We must read whatever data is available completely, as we are running in edge-triggered mode and won't get a notification again for the same data. */
                            int done = 0;

                            while (1)
                                {
                                    ssize_t count;
                                    char buf[512];

                                    count = read (events[i].data.fd, buf, sizeof(buf));
                                    if (count == -1)
                                        {
                                            /* If errno == EAGAIN, that means we have read all
                                               data. So go back to the main loop. */
                                            if (errno != EAGAIN)
                                                {
                                                    perror ("read");
                                                    done = 1;
                                                }
                                            break;
                                        }
                                    else if (count == 0)
                                        {
                                            /* End of file. The remote has closed the
                                               connection. */
                                            done = 1;
                                            break;
                                        }

                                    /* Write the buffer to standard output */
                                    //s = write (1, buf, count);
                                    //printf("recv size %d\n", count);
                                    s = write(events[i].data.fd, buf, count);
                                    if (s == -1)
                                        {
                                            perror ("write");
                                            abort ();
                                        }
                                }

                            if (done)
                                {
                                    //printf ("Closed connection on descriptor %d\n", events[i].data.fd);

                                    /* Closing the descriptor will make epoll remove it
                                       from the set of descriptors which are monitored. */
                                    close (events[i].data.fd);
                                }
                        }
                }
        }

    free (events);

    close (sfd);

    return EXIT_SUCCESS;
}


class TcpChannelTest : public testing::Test{
public:
    // TestSuite
    static void SetUpTestCase() {
        int  err;
        err = pthread_create(&g_tid, NULL, thread_echo_server, (void*)"");
        if (err != 0){
            printf("echo server not start up\n");
        }
        ont_platform_sleep(5);
    }

    static void TearDownTestCase() {
        pthread_cancel(g_tid);
    }

    // TestCase
    virtual void SetUp() {
        g_msg.m_send_buf = NULL;
        g_msg.m_send_size = 0;
        g_msg.m_recv_buf = NULL;
        g_msg.m_recv_size = 0;
    }

    virtual void TearDown() {
    }
    void start_echo_server();
};

class TcpChannelCreateTest : public TcpChannelTest{
};


/*����ont_tcp_channel_create*/
TEST_F(TcpChannelTest, TcpChannelCreateOk){
    ont_channel_interface_t iface;
    const char* owner_ctx = "nothing";
    EXPECT_EQ(ONT_ERR_OK, ont_tcp_channel_create(&iface, g_ip.c_str(), g_port, 1024, 1024, (char*)owner_ctx, 1000));

    CHECK_TCP_CHANNEL_T_PARA(iface);
}

/*����ont_tcp_channel_create, owner_ctx size = 0*/
TEST_F(TcpChannelTest, TcpChannelCreateOkOwnSize0){
    ont_channel_interface_t iface;
    const char* owner_ctx = "";
    EXPECT_EQ(ONT_ERR_OK, ont_tcp_channel_create(&iface, g_ip.c_str(), g_port, 1024, 1024, (char*)owner_ctx, 1000));
    CHECK_TCP_CHANNEL_T_PARA(iface);
}

/*����ont_tcp_channel_create,��������ȷ�����*/
TEST_F(TcpChannelTest, InvalidInterfaceAndAddr){
    EXPECT_EQ(1, ont_tcp_channel_create(NULL, g_ip.c_str(), g_port, 1024, 1024, NULL, 0));
    ont_channel_interface_t iface;
    EXPECT_EQ(1, ont_tcp_channel_create(&iface, NULL, g_port, 1024, 1024, NULL, 0));
}

/*����ont_tcp_channel_create,��������ȷ�����*/
TEST_F(TcpChannelTest, TcpChannelCreateParaCheck){
    ont_channel_interface_t iface;
    EXPECT_EQ(1, ont_tcp_channel_create(&iface, NULL, g_port, 1024, 1024, NULL, 0));
    EXPECT_EQ(1, ont_tcp_channel_create(&iface, NULL, g_port, 0, 1024, NULL, 0));
    EXPECT_EQ(1, ont_tcp_channel_create(&iface, NULL, g_port, 1024, 0, NULL, 0));
    EXPECT_EQ(1, ont_tcp_channel_create(&iface, NULL, g_port, 0, 0, NULL, 0));
    EXPECT_EQ(ONT_ERR_OK, ont_tcp_channel_create(&iface, g_ip.c_str(), g_port, 1024, 1024, NULL, 0));
    EXPECT_EQ(ONT_ERR_NOMEM, ont_tcp_channel_create(&iface, g_ip.c_str(), g_port, -1024, 1024, NULL, 1000));
    EXPECT_EQ(ONT_ERR_NOMEM, ont_tcp_channel_create(&iface, g_ip.c_str(), g_port, 1024, -1024, NULL, 1000));
    EXPECT_EQ(ONT_ERR_OK, ont_tcp_channel_create(&iface, g_ip.c_str(), g_port, 1024, 1024, NULL, -1000));
    EXPECT_EQ(1, ont_tcp_channel_create(&iface, NULL, g_port, ONT_SOCK_SEND_BUF_SIZE, 1024, NULL, 0));
    EXPECT_EQ(1, ont_tcp_channel_create(&iface, NULL, g_port, 1024, ONT_SOCK_SEND_BUF_SIZE, NULL, 0));
    EXPECT_EQ(1, ont_tcp_channel_create(&iface, NULL, g_port, ONT_SOCK_SEND_BUF_SIZE + 1, 1024, NULL, 0));
    EXPECT_EQ(1, ont_tcp_channel_create(&iface, NULL, g_port, ONT_SOCK_SEND_BUF_SIZE, ONT_SOCK_SEND_BUF_SIZE, NULL, 0));
}


void *tcp_channel_test_malloc_hook(size_t size){
    (void) (size);
    return NULL;
}
/*����ont_tcp_channel_create,mallocʧ��*/
TEST_F(TcpChannelTest, MallocTcpChannelFailed){
    DynamicStubManager mgr;
    mgr.MakeStub(malloc, tcp_channel_test_malloc_hook);

    ont_channel_interface_t iface;
    EXPECT_EQ(ONT_ERR_NOMEM, ont_tcp_channel_create(&iface, g_ip.c_str(), g_port, 1024, 1024, NULL, 0));

    mgr.UnmakeStub(malloc);
}


int tcp_channel_test_socket_hook(int domain, int type, int protocol){
    (void) domain;
    (void) type;
    (void) protocol;

    return -1;
}
/*����ont_tcp_channel_create,�޷�����socket*/
TEST_F(TcpChannelTest, CreateSocketFailed){
    DynamicStubManager mgr;
    mgr.MakeStub(socket, tcp_channel_test_socket_hook);

    ont_channel_interface_t iface;
    EXPECT_EQ(ONT_ERR_OK, ont_tcp_channel_create(&iface, g_ip.c_str(), g_port, 1024, 1024, NULL, 0));

    mgr.UnmakeStub(socket);
}

int ont_read_packet_cb(void *ctx,const char *buf,size_t buf_size,size_t *read_size){
    if(NULL == buf || buf_size == 0)
        return ONT_ERR_BADPARAM;

    memcpy(g_msg.m_recv_buf + g_msg.m_recv_size,
           buf,
           buf_size);
    g_msg.m_recv_size += buf_size;
    *read_size = buf_size;

    return ONT_ERR_OK;
}

/*����ont_tcp_channel_initilize�����Ǵ�ʱ�ܶ�����޷�ȷ��*/
TEST_F(TcpChannelTest, TcpChannelInitOk){
    ont_channel_interface_t iface;
    EXPECT_EQ(ONT_ERR_OK, ont_tcp_channel_create(&iface, g_ip.c_str(), g_port, 1024, 1024, NULL, 1000));
    EXPECT_EQ(ONT_ERR_OK, ont_tcp_channel_initilize(iface.channel, ont_read_packet_cb) );

    CHECK_TCP_CHANNEL_T_PARA_CB(iface, ont_read_packet_cb);

}

/*����ont_tcp_channel_initilize���Ƿ�����*/
TEST_F(TcpChannelTest, InvalidParTcpChannelInitFailed){
    ont_channel_interface_t iface;
    EXPECT_EQ(ONT_ERR_OK, ont_tcp_channel_create(&iface, g_ip.c_str(), g_port, 1024, 1024, NULL, 1000));
    EXPECT_EQ(ONT_ERR_BADPARAM, ont_tcp_channel_initilize(iface.channel, NULL) );
    EXPECT_EQ(ONT_ERR_BADPARAM, ont_tcp_channel_initilize(NULL, ont_read_packet_cb));
}

/*����ont_tcp_channel_connect*/
TEST_F(TcpChannelTest, TcpConnectOk){
    ont_channel_interface_t iface;
    EXPECT_EQ(ONT_ERR_OK, ont_tcp_channel_create(&iface, g_ip.c_str(), g_port, 1024, 1024, NULL, 1000));
    EXPECT_EQ(ONT_ERR_OK, ont_tcp_channel_initilize(iface.channel, ont_read_packet_cb) );
    int user_exit = 0;
    EXPECT_EQ(ONT_ERR_OK, ont_tcp_channel_connect(iface.channel, &user_exit));

    CHECK_TCP_CHANNEL_T_PARA_CB(iface, ont_read_packet_cb);

    tcp_channel_t* tcpc = (tcp_channel_t*)iface.channel;
    EXPECT_EQ(1000, tcpc->connect_timeout);
    EXPECT_EQ(ONT_ERR_OK, tcpc->conn_status);

}

/*�ر�socket���ӣ�����ont_tcp_channel_connect*/
TEST_F(TcpChannelTest, TcpConnectCreateSocketFailed){
    DynamicStubManager mgr;
    mgr.MakeStub(socket, tcp_channel_test_socket_hook);


    ont_channel_interface_t iface;
    EXPECT_EQ(ONT_ERR_OK, ont_tcp_channel_create(&iface, g_ip.c_str(), g_port, 1024, 1024, NULL, 1000));
    EXPECT_EQ(ONT_ERR_OK, ont_tcp_channel_initilize(iface.channel, ont_read_packet_cb) );
    int user_exit = 0;
    //���ӱ���˵������socket������connect�׶Σ���ǰ����
    EXPECT_EQ(ONT_ERR_SOCKET_OP_FAIL, ont_tcp_channel_connect(iface.channel, &user_exit));

    CHECK_TCP_CHANNEL_T_PARA_CB(iface, ont_read_packet_cb);

    tcp_channel_t* tcpc = (tcp_channel_t*)iface.channel;
    EXPECT_EQ(0, tcpc->connect_timeout);

    mgr.UnmakeStub(socket);

}


/*���Զ������*/
TEST_F(TcpChannelTest, TcpConnectMulTimes){
    ont_channel_interface_t iface;
    EXPECT_EQ(ONT_ERR_OK, ont_tcp_channel_create(&iface, g_ip.c_str(), g_port, 1024, 1024, NULL, 1000));
    EXPECT_EQ(ONT_ERR_OK, ont_tcp_channel_initilize(iface.channel, ont_read_packet_cb) );
    int user_exit = 0;
    EXPECT_EQ(ONT_ERR_OK, ont_tcp_channel_connect(iface.channel, &user_exit));
    EXPECT_EQ(ONT_ERR_OK, ont_tcp_channel_connect(iface.channel, &user_exit));
    user_exit = 1;
    EXPECT_EQ(ONT_ERR_DISCONNECTED_BY_USER, ont_tcp_channel_connect(iface.channel, &user_exit));


    CHECK_TCP_CHANNEL_T_PARA_CB(iface, ont_read_packet_cb);

    tcp_channel_t* tcpc = (tcp_channel_t*)iface.channel;
    EXPECT_EQ(1000, tcpc->connect_timeout);
    EXPECT_EQ(ONT_ERR_SOCKET_ISCONN, tcpc->conn_status); /*�������ֵ*/



}

/*�������ӣ�ʹ�÷Ƿ�����*/
TEST_F(TcpChannelTest, InValidParTcpConnectFailed){
    ont_channel_interface_t iface;
    EXPECT_EQ(ONT_ERR_OK, ont_tcp_channel_create(&iface, g_ip.c_str(), g_port, 1024, 1024, NULL, 1000));
    EXPECT_EQ(ONT_ERR_OK, ont_tcp_channel_initilize(iface.channel, ont_read_packet_cb) );
    int user_exit = 0;
    EXPECT_EQ(ONT_ERR_BADPARAM, ont_tcp_channel_connect(NULL, &user_exit) );
}

/*����ont_tcp_channel_write*/
TEST_F(TcpChannelTest, TcpWriteOk){
    ont_channel_interface_t iface;
    EXPECT_EQ(ONT_ERR_OK, ont_tcp_channel_create(&iface, g_ip.c_str(), g_port, 1024, 1024, NULL, 1000));
    EXPECT_EQ(ONT_ERR_OK, ont_tcp_channel_initilize(iface.channel, ont_read_packet_cb) );
    int user_exit = 0;
    EXPECT_EQ(ONT_ERR_OK, ont_tcp_channel_connect(iface.channel, &user_exit));
    char send_buf[1024];
    EXPECT_EQ(ONT_ERR_OK, ont_tcp_channel_write(iface.channel, send_buf, sizeof(send_buf)/sizeof(char)) );

    EXPECT_EQ(ONT_ERR_OK, ont_tcp_channel_write(iface.channel, send_buf, 1) );

    CHECK_TCP_CHANNEL_T_PARA_CB(iface, ont_read_packet_cb);
    tcp_channel_t* tcpc = (tcp_channel_t*)iface.channel;
    EXPECT_EQ(1000, tcpc->connect_timeout);
    EXPECT_EQ(ONT_ERR_OK, tcpc->conn_status);

}

/*����ont_tcp_channel_write,ʹ�÷Ƿ�����*/
TEST_F(TcpChannelTest, InValidParTcpWriteFailed){
    ont_channel_interface_t iface;
    EXPECT_EQ(ONT_ERR_OK, ont_tcp_channel_create(&iface, g_ip.c_str(), g_port, 1024, 1024, NULL, 1000));
    EXPECT_EQ(ONT_ERR_OK, ont_tcp_channel_initilize(iface.channel, ont_read_packet_cb) );
    int user_exit = 0;
    EXPECT_EQ(ONT_ERR_OK, ont_tcp_channel_connect(iface.channel, &user_exit));
    char send_buf[1024+1];
    EXPECT_EQ(1, ont_tcp_channel_write(iface.channel, NULL, 0));
    EXPECT_EQ(1, ont_tcp_channel_write(iface.channel, NULL, 1024));
    EXPECT_EQ(1, ont_tcp_channel_write(iface.channel, send_buf, 0));
    EXPECT_EQ(ONT_ERR_BUFFER_NOT_ENOUGH, ont_tcp_channel_write(iface.channel, send_buf, sizeof(send_buf)/sizeof(char)));
    tcp_channel_t* tc = (tcp_channel_t*)iface.channel;
    EXPECT_EQ(0, tc->buffer_out.current - tc->buffer_out.begin);
}

bool ont_ut_strcmp(const char *str1, const char *str2, size_t len){
    while(len > 0){
        if(*str1 != *str2)
            return false;
        str1++;
        str2++;
        len--;
    }
    return true;
}

/*���Է������buffer��1024�����жϷ��ͺͽ��յ�����һ��*/
TEST_F(TcpChannelTest, TcpSendFullOk){
    ont_channel_interface_t iface;
    EXPECT_EQ(ONT_ERR_OK, ont_tcp_channel_create(&iface, g_ip.c_str(), g_port, 1024, 1024, NULL, 1000));
    EXPECT_EQ(ONT_ERR_OK, ont_tcp_channel_initilize(iface.channel, ont_read_packet_cb) );
    int user_exit = 0;
    EXPECT_EQ(ONT_ERR_OK, ont_tcp_channel_connect(iface.channel, &user_exit));
    g_msg.m_send_size = 1024;
    g_msg.m_send_buf = (char*)ont_platform_malloc(g_msg.m_send_size);
    g_msg.m_recv_buf = (char*)ont_platform_malloc(g_msg.m_send_size);
    if(NULL == g_msg.m_send_buf)
        return;
    int i;
    for(i=0; i<1024; ++i)
        g_msg.m_send_buf[i] = i & 0xFF;
    EXPECT_EQ(ONT_ERR_OK, ont_tcp_channel_write(iface.channel, g_msg.m_send_buf, g_msg.m_send_size) );
    int err;
    do{
        err = ont_tcp_channel_process(iface.channel);
        if(ONT_ERR_SOCKET_OP_FAIL == err ||
            ONT_ERR_TIMEOUT == err)
            break;
        ont_platform_sleep(1);
    }while(g_msg.m_recv_size < g_msg.m_send_size);
    EXPECT_EQ(ONT_ERR_OK, err);
    EXPECT_EQ(g_msg.m_send_size, g_msg.m_recv_size);
    EXPECT_TRUE(ont_ut_strcmp(g_msg.m_send_buf, g_msg.m_recv_buf, g_msg.m_send_size));
    CHECK_TCP_CHANNEL_T_PARA_CB(iface, ont_read_packet_cb);
    if (g_msg.m_send_buf)
        ont_platform_free(g_msg.m_send_buf);
    if(g_msg.m_recv_buf)
        ont_platform_free(g_msg.m_recv_buf);

    tcp_channel_t* tcpc = (tcp_channel_t*)iface.channel;
    EXPECT_EQ(1000, tcpc->connect_timeout);
    EXPECT_EQ(ONT_ERR_OK, tcpc->conn_status);

}

/*���Է���1byte�����жϷ��ͺͽ��յ�����һ��*/
TEST_F(TcpChannelTest, TcpSend1Ok){
    ont_channel_interface_t iface;
    EXPECT_EQ(ONT_ERR_OK, ont_tcp_channel_create(&iface, g_ip.c_str(), g_port, 1024, 1024, NULL, 1000));
    EXPECT_EQ(ONT_ERR_OK, ont_tcp_channel_initilize(iface.channel, ont_read_packet_cb) );
    int user_exit = 0;
    EXPECT_EQ(ONT_ERR_OK, ont_tcp_channel_connect(iface.channel, &user_exit));
    g_msg.m_send_size = 1;
    g_msg.m_send_buf = (char*)ont_platform_malloc(g_msg.m_send_size);
    g_msg.m_recv_buf = (char*)ont_platform_malloc(g_msg.m_send_size);
    if(NULL == g_msg.m_send_buf)
        return;
    g_msg.m_send_buf[0] = 0x00;
    EXPECT_EQ(ONT_ERR_OK, ont_tcp_channel_write(iface.channel, g_msg.m_send_buf, g_msg.m_send_size) );
    int err;
    do{
        err = ont_tcp_channel_process(iface.channel);
        if(ONT_ERR_SOCKET_OP_FAIL == err ||
            ONT_ERR_TIMEOUT == err)
            break;
        ont_platform_sleep(1);
    }while(g_msg.m_recv_size < g_msg.m_send_size);
    EXPECT_EQ(ONT_ERR_OK, err);
    EXPECT_EQ(g_msg.m_send_size, g_msg.m_recv_size);
    EXPECT_TRUE(ont_ut_strcmp(g_msg.m_send_buf, g_msg.m_recv_buf, g_msg.m_send_size));
    CHECK_TCP_CHANNEL_T_PARA_CB(iface, ont_read_packet_cb);
    if (g_msg.m_send_buf)
        ont_platform_free(g_msg.m_send_buf);
    if(g_msg.m_recv_buf)
        ont_platform_free(g_msg.m_recv_buf);

    tcp_channel_t* tcpc = (tcp_channel_t*)iface.channel;
    EXPECT_EQ(1000, tcpc->connect_timeout);
    EXPECT_EQ(ONT_ERR_OK, tcpc->conn_status);

}

/*���Զ�η��ͣ���Ȼ�ܹ���������*/
TEST_F(TcpChannelTest, TcpSendMultiTimes){
    ont_channel_interface_t iface;
    EXPECT_EQ(ONT_ERR_OK, ont_tcp_channel_create(&iface, g_ip.c_str(), g_port, 1024, 1024, NULL, 1000));
    EXPECT_EQ(ONT_ERR_OK, ont_tcp_channel_initilize(iface.channel, ont_read_packet_cb) );
    int user_exit = 0;
    EXPECT_EQ(ONT_ERR_OK, ont_tcp_channel_connect(iface.channel, &user_exit));
    g_msg.m_send_size = 1024;
    g_msg.m_send_buf = (char*)ont_platform_malloc(g_msg.m_send_size);
    g_msg.m_recv_buf = (char*)ont_platform_malloc(g_msg.m_send_size);
    if(NULL == g_msg.m_send_buf)
        return;
    int i,j;
    for(i=1; i<10; ++i){
        g_msg.m_send_size = i;
        g_msg.m_recv_size = 0;
        for(j=0; j<i; ++j)
            g_msg.m_send_buf[j] = 0xFF & i;
        EXPECT_EQ(ONT_ERR_OK, ont_tcp_channel_write(iface.channel, g_msg.m_send_buf, g_msg.m_send_size));
        int err;
        do{
            err = ont_tcp_channel_process(iface.channel);
            if(ONT_ERR_SOCKET_OP_FAIL == err ||
               ONT_ERR_TIMEOUT == err)
                break;
            ont_platform_sleep(1);
        }while(g_msg.m_recv_size < g_msg.m_send_size);
        EXPECT_EQ(ONT_ERR_OK, err);
        EXPECT_EQ(g_msg.m_send_size, g_msg.m_recv_size);
        EXPECT_TRUE(ont_ut_strcmp(g_msg.m_send_buf, g_msg.m_recv_buf, g_msg.m_send_size));
        CHECK_TCP_CHANNEL_T_PARA_CB(iface, ont_read_packet_cb);
    }
    if (g_msg.m_send_buf)
        ont_platform_free(g_msg.m_send_buf);
    if(g_msg.m_recv_buf)
        ont_platform_free(g_msg.m_recv_buf);

    tcp_channel_t* tcpc = (tcp_channel_t*)iface.channel;
    EXPECT_EQ(1000, tcpc->connect_timeout);
    EXPECT_EQ(ONT_ERR_OK, tcpc->conn_status);

}

/*����ont_tcp_channel_process,�Ƿ����������*/
TEST_F(TcpChannelTest, InValidParTcpProcessFailed){
    ont_channel_interface_t iface;
    EXPECT_EQ(ONT_ERR_OK, ont_tcp_channel_create(&iface, g_ip.c_str(), g_port, 1024, 1024, NULL, 1000));
    EXPECT_EQ(ONT_ERR_OK, ont_tcp_channel_initilize(iface.channel, ont_read_packet_cb) );
    int user_exit = 0;
    EXPECT_EQ(ONT_ERR_OK, ont_tcp_channel_connect(iface.channel, &user_exit));
    g_msg.m_send_size = 1;
    g_msg.m_send_buf = (char*)ont_platform_malloc(g_msg.m_send_size);
    g_msg.m_recv_buf = (char*)ont_platform_malloc(g_msg.m_send_size);
    if(NULL == g_msg.m_send_buf)
        return;
    g_msg.m_send_buf[0] = 0x00;
    EXPECT_EQ(ONT_ERR_OK, ont_tcp_channel_write(iface.channel, g_msg.m_send_buf, g_msg.m_send_size) );
    EXPECT_EQ(1, ont_tcp_channel_process(NULL));
}

/*����ont_platform_deinitilize*/
TEST_F(TcpChannelTest, TcpDeinitOk){
    ont_channel_interface_t iface;
    EXPECT_EQ(ONT_ERR_OK, ont_tcp_channel_create(&iface, g_ip.c_str(), g_port, 1024, 1024, NULL, 1000));
    EXPECT_EQ(ONT_ERR_OK, ont_tcp_channel_initilize(iface.channel, ont_read_packet_cb) );
    int user_exit = 0;
    EXPECT_EQ(ONT_ERR_OK, ont_tcp_channel_connect(iface.channel, &user_exit));
    g_msg.m_send_size = 1;
    g_msg.m_send_buf = (char*)ont_platform_malloc(g_msg.m_send_size);
    g_msg.m_recv_buf = (char*)ont_platform_malloc(g_msg.m_send_size);
    if(NULL == g_msg.m_send_buf)
        return;
    g_msg.m_send_buf[0] = 0x00;
    EXPECT_EQ(ONT_ERR_OK, ont_tcp_channel_write(iface.channel, g_msg.m_send_buf, g_msg.m_send_size) );
    int err;
    do{
        err = ont_tcp_channel_process(iface.channel);
        if(ONT_ERR_SOCKET_OP_FAIL == err ||
            ONT_ERR_TIMEOUT == err)
            break;
        ont_platform_sleep(1);
    }while(g_msg.m_recv_size < g_msg.m_send_size);
    EXPECT_EQ(ONT_ERR_OK, err);
    EXPECT_EQ(g_msg.m_send_size, g_msg.m_recv_size);
    EXPECT_TRUE(ont_ut_strcmp(g_msg.m_send_buf, g_msg.m_recv_buf, g_msg.m_send_size));
    if (g_msg.m_send_buf)
        ont_platform_free(g_msg.m_send_buf);
    if(g_msg.m_recv_buf)
        ont_platform_free(g_msg.m_recv_buf);
    EXPECT_EQ(ONT_ERR_OK, ont_tcp_channel_deinitilize(iface.channel));

    EXPECT_EQ(iface.fn_initilize, ont_tcp_channel_initilize);
    EXPECT_EQ(iface.fn_deinitilize, ont_tcp_channel_deinitilize);
    EXPECT_EQ(iface.fn_write, ont_tcp_channel_write);
    EXPECT_EQ(iface.fn_process, ont_tcp_channel_process);
    EXPECT_EQ(iface.fn_connect, ont_tcp_channel_connect);
    EXPECT_EQ(iface.fn_cb_read_packet, ont_read_packet_cb);
    tcp_channel_t* tc = (tcp_channel_t*)iface.channel;
    EXPECT_EQ(NULL, tc->channel);
    EXPECT_EQ(0, strcmp(tc->addr, g_ip.c_str()));
    EXPECT_EQ(tc->port, g_port);
    EXPECT_EQ(1000, tc->connect_timeout);
    EXPECT_EQ(tc->timeout, 1000);
    EXPECT_EQ(ONT_ERR_OK, tc->conn_status);
    EXPECT_TRUE(NULL == tc->buffer_in.begin);
    EXPECT_TRUE(NULL == tc->buffer_out.begin);
    EXPECT_STREQ(NULL, (char*)tc->owner_ctx);

}

/*����ont_platform_deinitilize,�Ƿ����Ե����*/
TEST_F(TcpChannelTest, InValidParTcpDeinitFailed){
ont_channel_interface_t iface;
    EXPECT_EQ(ONT_ERR_OK, ont_tcp_channel_create(&iface, g_ip.c_str(), g_port, 1024, 1024, NULL, 1000));
    EXPECT_EQ(ONT_ERR_OK, ont_tcp_channel_initilize(iface.channel, ont_read_packet_cb) );
    int user_exit = 0;
    EXPECT_EQ(ONT_ERR_OK, ont_tcp_channel_connect(iface.channel, &user_exit));
    g_msg.m_send_size = 1;
    g_msg.m_send_buf = (char*)ont_platform_malloc(g_msg.m_send_size);
    g_msg.m_recv_buf = (char*)ont_platform_malloc(g_msg.m_send_size);
    if(NULL == g_msg.m_send_buf)
        return;
    g_msg.m_send_buf[0] = 0x00;
    EXPECT_EQ(ONT_ERR_OK, ont_tcp_channel_write(iface.channel, g_msg.m_send_buf, g_msg.m_send_size) );
    int err;
    do{
        err = ont_tcp_channel_process(iface.channel);
        if(ONT_ERR_SOCKET_OP_FAIL == err ||
            ONT_ERR_TIMEOUT == err)
            break;
        ont_platform_sleep(1);
    }while(g_msg.m_recv_size < g_msg.m_send_size);
    EXPECT_EQ(ONT_ERR_OK, err);
    EXPECT_EQ(g_msg.m_send_size, g_msg.m_recv_size);
    EXPECT_TRUE(ont_ut_strcmp(g_msg.m_send_buf, g_msg.m_recv_buf, g_msg.m_send_size));
    if (g_msg.m_send_buf)
        ont_platform_free(g_msg.m_send_buf);
    if(g_msg.m_recv_buf)
        ont_platform_free(g_msg.m_recv_buf);
    EXPECT_EQ(ONT_ERR_BADPARAM, ont_tcp_channel_deinitilize(NULL));

    CHECK_TCP_CHANNEL_T_PARA_CB(iface, ont_read_packet_cb);
}


/*û��initilize����ȱ��call back����������process �쳣*/
/*
TEST_F(TcpChannelTest, NoInit){
    ont_channel_interface_t iface;
    EXPECT_EQ(ONT_ERR_OK, ont_tcp_channel_create(&iface, g_ip.c_str(), g_port, 1024, 1024, NULL, 1000));
    //EXPECT_EQ(ONT_ERR_OK, ont_tcp_channel_initilize(iface.channel, ont_read_packet_cb) );
    int user_exit = 0;
    EXPECT_EQ(ONT_ERR_OK, ont_tcp_channel_connect(iface.channel, &user_exit));
    g_msg.m_send_size = 1;
    g_msg.m_send_buf = (char*)ont_platform_malloc(g_msg.m_send_size);
    g_msg.m_recv_buf = (char*)ont_platform_malloc(g_msg.m_send_size);
    if(NULL == g_msg.m_send_buf)
        return;
    g_msg.m_send_buf[0] = 0x00;
    EXPECT_EQ(ONT_ERR_OK, ont_tcp_channel_write(iface.channel, g_msg.m_send_buf, g_msg.m_send_size) );
    EXPECT_ANY_THROW( ont_tcp_channel_process(iface.channel));
}
*/

/*����û�����ӵ�����·�������*/
TEST_F(TcpChannelTest, NoConnect){
    ont_channel_interface_t iface;
    EXPECT_EQ(ONT_ERR_OK, ont_tcp_channel_create(&iface, g_ip.c_str(), g_port, 1024, 1024, NULL, 1000));
    EXPECT_EQ(ONT_ERR_OK, ont_tcp_channel_initilize(iface.channel, ont_read_packet_cb) );
    //EXPECT_EQ(ONT_ERR_OK, ont_tcp_channel_connect(iface.channel));
    g_msg.m_send_size = 1;
    g_msg.m_send_buf = (char*)ont_platform_malloc(g_msg.m_send_size);
    g_msg.m_recv_buf = (char*)ont_platform_malloc(g_msg.m_send_size);
    if(NULL == g_msg.m_send_buf)
        return;
    g_msg.m_send_buf[0] = 0x00;
    tcp_channel_t* tc = (tcp_channel_t*)iface.channel;

    //��ǰ��ont_tcp_channel_connect,��һ��write��ɹ�����ʵ�ǰ�Ҫ���͵�����д��buffer����ǵڶ��ξͻ�ʧ�ܡ����ڳ��ˣ�û��ont_tcp_channel_connet,��һ��write�ͻ�ʧ��
    EXPECT_EQ(ONT_ERR_SOCKET_OP_FAIL, ont_tcp_channel_write(iface.channel, g_msg.m_send_buf, g_msg.m_send_size) );
    EXPECT_EQ(0, tc->buffer_out.current - tc->buffer_out.begin);
    // send again
    EXPECT_EQ(ONT_ERR_SOCKET_OP_FAIL, ont_tcp_channel_write(iface.channel, g_msg.m_send_buf, g_msg.m_send_size) );
    EXPECT_EQ(ONT_ERR_DISCONNECTED_BY_USER, ont_tcp_channel_process(iface.channel));

    //�������ӣ��ڴ��ٴη����ܹ��ɹ�
    int user_exit = 0;
    EXPECT_EQ(ONT_ERR_OK, ont_tcp_channel_connect(iface.channel, &user_exit));
    EXPECT_EQ(ONT_ERR_OK, ont_tcp_channel_write(iface.channel,
                                                g_msg.m_send_buf,
                                                g_msg.m_send_size));
}

/*�û��ֶ��˳���write��Ȼ�ɹ�������process�ͻ�ʧ��*/
TEST_F(TcpChannelTest, UserExit){
    ont_channel_interface_t iface;
    EXPECT_EQ(ONT_ERR_OK, ont_tcp_channel_create(&iface, g_ip.c_str(), g_port, 1024, 1024, NULL, 1000));
    EXPECT_EQ(ONT_ERR_OK, ont_tcp_channel_initilize(iface.channel, ont_read_packet_cb) );
    int user_exit = 0;
    EXPECT_EQ(ONT_ERR_OK, ont_tcp_channel_connect(iface.channel, &user_exit));
    g_msg.m_send_size = 1;
    g_msg.m_send_buf = (char*)ont_platform_malloc(g_msg.m_send_size);
    g_msg.m_recv_buf = (char*)ont_platform_malloc(g_msg.m_send_size);
    if(NULL == g_msg.m_send_buf)
        return;
    g_msg.m_send_buf[0] = 0x00;
    EXPECT_EQ(ONT_ERR_OK, ont_tcp_channel_write(iface.channel, g_msg.m_send_buf, g_msg.m_send_size) );

    tcp_channel_t* tc = (tcp_channel_t*)iface.channel;
    user_exit = 1;
    EXPECT_EQ(ONT_ERR_DISCONNECTED_BY_USER, ont_tcp_channel_connect(iface.channel, &user_exit));
    EXPECT_EQ(ONT_ERR_OK, ont_tcp_channel_write(iface.channel, g_msg.m_send_buf, g_msg.m_send_size) );
    EXPECT_EQ(0, tc->buffer_out.current - tc->buffer_out.begin);
    EXPECT_EQ(ONT_ERR_DISCONNECTED_BY_USER, ont_tcp_channel_process(iface.channel));
}

/*��deinilize���ٴ�����*/
TEST_F(TcpChannelTest, TcpConnectAfterDeinit){
    ont_channel_interface_t iface;
    EXPECT_EQ(ONT_ERR_OK, ont_tcp_channel_create(&iface, g_ip.c_str(), g_port, 1024, 1024, NULL, 1000));
    EXPECT_EQ(ONT_ERR_OK, ont_tcp_channel_initilize(iface.channel, ont_read_packet_cb) );
    int user_exit = 0;
    EXPECT_EQ(ONT_ERR_OK, ont_tcp_channel_connect(iface.channel, &user_exit));
    g_msg.m_send_size = 1;
    g_msg.m_send_buf = (char*)ont_platform_malloc(g_msg.m_send_size);
    g_msg.m_recv_buf = (char*)ont_platform_malloc(g_msg.m_send_size);
    if(NULL == g_msg.m_send_buf)
        return;
    g_msg.m_send_buf[0] = 0x00;
    EXPECT_EQ(ONT_ERR_OK, ont_tcp_channel_write(iface.channel, g_msg.m_send_buf, g_msg.m_send_size) );
    int err;
    do{
        err = ont_tcp_channel_process(iface.channel);
        if(ONT_ERR_SOCKET_OP_FAIL == err ||
            ONT_ERR_TIMEOUT == err)
            break;
        ont_platform_sleep(1);
    }while(g_msg.m_recv_size < g_msg.m_send_size);
    EXPECT_EQ(ONT_ERR_OK, err);
    EXPECT_EQ(g_msg.m_send_size, g_msg.m_recv_size);
    EXPECT_TRUE(ont_ut_strcmp(g_msg.m_send_buf, g_msg.m_recv_buf, g_msg.m_send_size));
    if (g_msg.m_send_buf)
        ont_platform_free(g_msg.m_send_buf);
    if(g_msg.m_recv_buf)
        ont_platform_free(g_msg.m_recv_buf);
    EXPECT_EQ(ONT_ERR_OK, ont_tcp_channel_deinitilize(iface.channel));
    EXPECT_EQ(ONT_ERR_SOCKET_OP_FAIL, ont_tcp_channel_connect(iface.channel, &user_exit));

}


ssize_t tcp_channel_test_send_hook(int sockfd, const void */*buf*/, size_t nbytes,int flags){
    (void) sockfd;
    //(void*) buf;
    (void) nbytes;
    (void) flags;
    return -1;
}

/*�����޷��������ݵ����*/
TEST_F(TcpChannelTest, TcpWriteFailed){
    ont_channel_interface_t iface;
    EXPECT_EQ(ONT_ERR_OK, ont_tcp_channel_create(&iface, g_ip.c_str(), g_port, 1024, 1024, NULL, 1000));
    EXPECT_EQ(ONT_ERR_OK, ont_tcp_channel_initilize(iface.channel, ont_read_packet_cb) );
    int user_exit = 0;
    EXPECT_EQ(ONT_ERR_OK, ont_tcp_channel_connect(iface.channel, &user_exit));
    g_msg.m_send_size = 2046;
    g_msg.m_send_buf = (char*)ont_platform_malloc(g_msg.m_send_size);
    g_msg.m_recv_buf = (char*)ont_platform_malloc(g_msg.m_send_size);
    if(NULL == g_msg.m_send_buf)
        return;
    for(size_t i=0;i<g_msg.m_send_size;++i)
        g_msg.m_send_buf[i] = 0xFF & i;
    tcp_channel_t* tc = (tcp_channel_t*)iface.channel;

    DynamicStubManager mgr;
    mgr.MakeStub(send, tcp_channel_test_send_hook);
    EXPECT_EQ(ONT_ERR_SOCKET_OP_FAIL, ont_tcp_channel_write(iface.channel, g_msg.m_send_buf, 1) );
    //��ǰ��write���ɹ���д��buffer������ֱ�Ӿͷ��ش������ԾͲ����жϴ�������
    //EXPECT_TRUE( EINTR == errno || EAGAIN == errno );
    EXPECT_EQ(0, tc->buffer_out.current - tc->buffer_out.begin);
    EXPECT_EQ(ONT_ERR_SOCKET_OP_FAIL, ont_tcp_channel_write(iface.channel,
                                                g_msg.m_send_buf+1,
                                                100));
    mgr.UnmakeStub(send);

}

ssize_t tcp_channel_test_recv_hook(int sockfd, void */*buf*/, size_t nbytes,int flags){
    (void) sockfd;
    (void) nbytes;
    (void) flags;
    return -1;
}

/*�����޷��յ����*/
TEST_F(TcpChannelTest, TcpRecvFailed){
    ont_channel_interface_t iface;
    EXPECT_EQ(ONT_ERR_OK, ont_tcp_channel_create(&iface, g_ip.c_str(), g_port, 1024, 1024, NULL, 1000));
    EXPECT_EQ(ONT_ERR_OK, ont_tcp_channel_initilize(iface.channel, ont_read_packet_cb) );
    int user_exit = 0;
    EXPECT_EQ(ONT_ERR_OK, ont_tcp_channel_connect(iface.channel, &user_exit));
    g_msg.m_send_size = 1;
    g_msg.m_send_buf = (char*)ont_platform_malloc(g_msg.m_send_size);
    g_msg.m_recv_buf = (char*)ont_platform_malloc(g_msg.m_send_size);
    if(NULL == g_msg.m_send_buf)
        return;
    g_msg.m_send_buf[0] = 0x00;
    EXPECT_EQ(ONT_ERR_OK, ont_tcp_channel_write(iface.channel, g_msg.m_send_buf, g_msg.m_send_size) );

    //fixme:Ӧ���ǲ���д�����ش���
    DynamicStubManager mgr;
    mgr.MakeStub(recv, tcp_channel_test_recv_hook);
    EXPECT_EQ(ONT_ERR_SOCKET_OP_FAIL, ont_tcp_channel_process(iface.channel));
    mgr.UnmakeStub(recv);

}

/*ÿ�η���ֻ������һ���ֽ�*/
ssize_t tcp_channel_test_send_1byte_hook(int sockfd, const void */*buf*/, size_t nbytes,int flags){
    (void) sockfd;
    (void) nbytes;
    (void) flags;
    return 1;
}

/*����ÿ�η��ͣ�ֻ�ܷ���1�ֽڣ��ײ���Ҫ���send���ܷ������*/
TEST_F(TcpChannelTest, TcpWrite1byte){
    ont_channel_interface_t iface;
    EXPECT_EQ(ONT_ERR_OK, ont_tcp_channel_create(&iface, g_ip.c_str(), g_port, 1024, 1024, NULL, 1000));
    EXPECT_EQ(ONT_ERR_OK, ont_tcp_channel_initilize(iface.channel, ont_read_packet_cb) );
    int user_exit = 0;
    EXPECT_EQ(ONT_ERR_OK, ont_tcp_channel_connect(iface.channel, &user_exit));
    g_msg.m_send_size = 2046;
    g_msg.m_send_buf = (char*)ont_platform_malloc(g_msg.m_send_size);
    g_msg.m_recv_buf = (char*)ont_platform_malloc(g_msg.m_send_size);
    if(NULL == g_msg.m_send_buf)
        return;
    for(size_t i=0;i<g_msg.m_send_size;++i)
        g_msg.m_send_buf[i] = 0xFF & i;
    tcp_channel_t* tc = (tcp_channel_t*)iface.channel;

    DynamicStubManager mgr;
    mgr.MakeStub(send, tcp_channel_test_send_1byte_hook);
    EXPECT_EQ(ONT_ERR_OK, ont_tcp_channel_write(iface.channel, g_msg.m_send_buf, 10) );
    //��ǰ��,send 10�ֽڣ�ʵ��ֻ������1�ֽڣ�����9�ֽ���buffer������ǣ��Ὣ�������ݷ����꣬���˳�
    EXPECT_EQ(0, tc->buffer_out.current - tc->buffer_out.begin);
    //�ٴη���
    EXPECT_EQ(ONT_ERR_OK, ont_tcp_channel_write(iface.channel,
                                                g_msg.m_send_buf+10,
                                                10));
    mgr.UnmakeStub(send);

}



ssize_t tcp_channel_test_send_partial_hook(int sockfd, const void */*buf*/, size_t nbytes,int flags){
    (void) sockfd;
    (void) nbytes;
    (void) flags;
    if(nbytes < 5 && nbytes > 1)
        return -1;
    return 1;
}
/*ÿ�η���ֻ�ܷ���һ�������ݣ���ʹ�ٴε���sendҲ����ɹ�*/
TEST_F(TcpChannelTest, TcpWritePartial){
    ont_channel_interface_t iface;
    EXPECT_EQ(ONT_ERR_OK, ont_tcp_channel_create(&iface, g_ip.c_str(), g_port, 1024, 1024, NULL, 1000));
    EXPECT_EQ(ONT_ERR_OK, ont_tcp_channel_initilize(iface.channel, ont_read_packet_cb) );
    int user_exit = 0;
    EXPECT_EQ(ONT_ERR_OK, ont_tcp_channel_connect(iface.channel, &user_exit));
    g_msg.m_send_size = 2046;
    g_msg.m_send_buf = (char*)ont_platform_malloc(g_msg.m_send_size);
    g_msg.m_recv_buf = (char*)ont_platform_malloc(g_msg.m_send_size);
    if(NULL == g_msg.m_send_buf)
        return;
    for(size_t i=0;i<g_msg.m_send_size;++i)
        g_msg.m_send_buf[i] = 0xFF & i;
    tcp_channel_t* tc = (tcp_channel_t*)iface.channel;

    DynamicStubManager mgr;
    mgr.MakeStub(send, tcp_channel_test_send_partial_hook);
    //10���ֽڣ�ֻ�ܷ���6���ֽڣ��Ժ��ʧ���ˡ����ڲ����ʣ���4���ֽڷŵ�buffer�ֱ�ӷ���ʧ��
    EXPECT_EQ(ONT_ERR_SOCKET_OP_FAIL, ont_tcp_channel_write(iface.channel, g_msg.m_send_buf, 10) );
    EXPECT_EQ(0, tc->buffer_out.current - tc->buffer_out.begin);
    mgr.UnmakeStub(send);
    //��һ��ʧ�ܣ��ڶ���Ҳ����ɹ�
    EXPECT_EQ(ONT_ERR_SOCKET_OP_FAIL, ont_tcp_channel_write(iface.channel,
                                                g_msg.m_send_buf,
                                                1));
    //fixme:��һ�η���ʧ�ܣ��ͱ����ٴε���ont_platform_connect
    EXPECT_EQ(ONT_ERR_OK, ont_tcp_channel_connect(iface.channel, &user_exit));
    EXPECT_EQ(ONT_ERR_OK, ont_tcp_channel_write(iface.channel,
                                                g_msg.m_send_buf,
                                                1));

}

//fixme:socket����������
