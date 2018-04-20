
#include <rados/librados.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

#define OID "nfsv41.clientid.directory"

rados_t c_cluster;
rados_ioctx_t c_io;

int init_rados()
{
  std::cout << __func__ << std::endl;
  char user_name[] = "client.admin";
  const char *cluster_name = "ceph" ;
  //lclient->client_messenger->cct->_conf->cluster.c_str();
  int64_t poolid = 135;
  //lclient->mds->mdsmap->get_metadata_pool();
  int ret = rados_create2(&c_cluster, cluster_name, user_name, 0);
  if (ret < 0) {
    std::cout << __func__ << ": create rados failed ret = " << ret << std::endl;
    return ret;
  }

  // init conf
  //librados::RadosClient *client = (librados::RadosClient *)c_cluster;
  //client->cct->_conf = lclient->client_messenger->cct->_conf;
  
  rados_conf_read_file(c_cluster, "/etc/ceph/ceph.conf");
  if (ret < 0) {
    std::cout << __func__ << ": read conf file failed ret = " << ret << std::endl;
    return ret;
  }
  

  ret = rados_connect(c_cluster);
  if (ret < 0) {
    std::cout << __func__ << ": create rados failed ret = " << ret << std::endl;
    return ret;
  }

  ret = rados_ioctx_create2(c_cluster, poolid, &c_io);
  if (ret < 0) {
    std::cout << __func__ << ": create ioctx failed ret = " << std::endl;
    return ret;
  }

  return 0;
}

int read_all_kvs_from_rados(const char *oid)
{
  std::cout << __func__ << std::endl;
  rados_read_op_t op = rados_create_read_op();
  int ret;
  int rval;
  if (!op) {
    std::cout << __func__ << ": create read op failed" << std::endl;
    return -1;
  }
  uint64_t count = 128;
  rados_omap_iter_t itr;
  rados_read_op_assert_exists(op);
  //rados_read_op_omap_get_vals(op, NULL, NULL, NFSV41_MAX_CLIENT_COUNT, &itr, &rval); 
  std::cout << __func__ << ": will read "<< count << " kv pairs" << std::endl;
  rados_read_op_omap_get_vals(op, NULL, NULL, count, &itr, &rval);
  ret = rados_read_op_operate(op, c_io, oid, 0);
  rados_release_read_op(op);
  if (ret < 0) {
    std::cout << __func__ << ": read op operate failed, ret = " << ret << std::endl;
    return ret;
  }
  std::cout << __func__ << ": get kvs rval = " << rval << std::endl;
  if (rval <= 0) {
    std::cout << __func__ << ": get vals failed, rval = " << rval << std::endl;
    //return -1; 
  }
  // kvs need to be copied ... 

  char *keyi = NULL, *vali = NULL, *valx = NULL;
  size_t leni;
  uint64_t clientid = 0;
  uint32_t ipaddr = 0;
  do {
    rados_omap_get_next(itr, &keyi, &vali, &leni);
    if (keyi == NULL) {
      std::cout << __func__ << ": reach kv end" << std::endl;
      break;
    }
    clientid = strtoull(keyi, NULL, 10);
    if (clientid == ULLONG_MAX) {
      return -ERANGE;
    }
    ipaddr = strtoull(vali, NULL, 16);
    if (ipaddr == ULLONG_MAX) {
      return -ERANGE;
    }
/*
    valx = new char[leni+1];
    memcpy(valx, vali, leni);
    valx[leni+1] = '\0';
*/
    std::cout << __func__ << ": <" << clientid << ", " << ipaddr << ">" << std::endl;
  } while(keyi != NULL);
  rados_omap_get_end(itr);
  return 0;
}

void shutdown_rados()
{
  rados_ioctx_destroy(c_io);
  rados_shutdown(c_cluster);
}

int main(int argc, char *argv[])
{
  if (init_rados() < 0)
    return -1;
  read_all_kvs_from_rados(OID); 
  shutdown_rados();
  return 0;
}
