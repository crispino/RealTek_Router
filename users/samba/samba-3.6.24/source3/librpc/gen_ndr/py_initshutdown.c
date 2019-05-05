
/* Python wrapper functions auto-generated by pidl */
#include <Python.h>
#include "includes.h"
#include <pytalloc.h>
#include "librpc/rpc/pyrpc.h"
#include "librpc/rpc/pyrpc_util.h"
#include "librpc/gen_ndr/ndr_initshutdown.h"
#include "librpc/gen_ndr/ndr_initshutdown_c.h"

#include "librpc/gen_ndr/lsa.h"
staticforward PyTypeObject initshutdown_InterfaceType;

void initinitshutdown(void);static PyTypeObject *lsa_StringLarge_Type;
static PyTypeObject *ClientConnection_Type;

static bool pack_py_initshutdown_Init_args_in(PyObject *args, PyObject *kwargs, struct initshutdown_Init *r)
{
	PyObject *py_hostname;
	PyObject *py_message;
	PyObject *py_timeout;
	PyObject *py_force_apps;
	PyObject *py_do_reboot;
	const char *kwnames[] = {
		"hostname", "message", "timeout", "force_apps", "do_reboot", NULL
	};

	if (!PyArg_ParseTupleAndKeywords(args, kwargs, "OOOOO:initshutdown_Init", discard_const_p(char *, kwnames), &py_hostname, &py_message, &py_timeout, &py_force_apps, &py_do_reboot)) {
		return false;
	}

	if (py_hostname == Py_None) {
		r->in.hostname = NULL;
	} else {
		r->in.hostname = talloc_ptrtype(r, r->in.hostname);
		PY_CHECK_TYPE(&PyInt_Type, py_hostname, return false;);
		*r->in.hostname = PyInt_AsLong(py_hostname);
	}
	if (py_message == Py_None) {
		r->in.message = NULL;
	} else {
		r->in.message = NULL;
		PY_CHECK_TYPE(lsa_StringLarge_Type, py_message, return false;);
		if (talloc_reference(r, py_talloc_get_mem_ctx(py_message)) == NULL) {
			PyErr_NoMemory();
			return false;
		}
		r->in.message = (struct lsa_StringLarge *)py_talloc_get_ptr(py_message);
	}
	PY_CHECK_TYPE(&PyInt_Type, py_timeout, return false;);
	r->in.timeout = PyInt_AsLong(py_timeout);
	PY_CHECK_TYPE(&PyInt_Type, py_force_apps, return false;);
	r->in.force_apps = PyInt_AsLong(py_force_apps);
	PY_CHECK_TYPE(&PyInt_Type, py_do_reboot, return false;);
	r->in.do_reboot = PyInt_AsLong(py_do_reboot);
	return true;
}

static PyObject *unpack_py_initshutdown_Init_args_out(struct initshutdown_Init *r)
{
	PyObject *result;
	result = Py_None;
	Py_INCREF(result);
	if (!W_ERROR_IS_OK(r->out.result)) {
		PyErr_SetWERROR(r->out.result);
		return NULL;
	}

	return result;
}

static bool pack_py_initshutdown_Abort_args_in(PyObject *args, PyObject *kwargs, struct initshutdown_Abort *r)
{
	PyObject *py_server;
	const char *kwnames[] = {
		"server", NULL
	};

	if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O:initshutdown_Abort", discard_const_p(char *, kwnames), &py_server)) {
		return false;
	}

	if (py_server == Py_None) {
		r->in.server = NULL;
	} else {
		r->in.server = talloc_ptrtype(r, r->in.server);
		PY_CHECK_TYPE(&PyInt_Type, py_server, return false;);
		*r->in.server = PyInt_AsLong(py_server);
	}
	return true;
}

static PyObject *unpack_py_initshutdown_Abort_args_out(struct initshutdown_Abort *r)
{
	PyObject *result;
	result = Py_None;
	Py_INCREF(result);
	if (!W_ERROR_IS_OK(r->out.result)) {
		PyErr_SetWERROR(r->out.result);
		return NULL;
	}

	return result;
}

static bool pack_py_initshutdown_InitEx_args_in(PyObject *args, PyObject *kwargs, struct initshutdown_InitEx *r)
{
	PyObject *py_hostname;
	PyObject *py_message;
	PyObject *py_timeout;
	PyObject *py_force_apps;
	PyObject *py_do_reboot;
	PyObject *py_reason;
	const char *kwnames[] = {
		"hostname", "message", "timeout", "force_apps", "do_reboot", "reason", NULL
	};

	if (!PyArg_ParseTupleAndKeywords(args, kwargs, "OOOOOO:initshutdown_InitEx", discard_const_p(char *, kwnames), &py_hostname, &py_message, &py_timeout, &py_force_apps, &py_do_reboot, &py_reason)) {
		return false;
	}

	if (py_hostname == Py_None) {
		r->in.hostname = NULL;
	} else {
		r->in.hostname = talloc_ptrtype(r, r->in.hostname);
		PY_CHECK_TYPE(&PyInt_Type, py_hostname, return false;);
		*r->in.hostname = PyInt_AsLong(py_hostname);
	}
	if (py_message == Py_None) {
		r->in.message = NULL;
	} else {
		r->in.message = NULL;
		PY_CHECK_TYPE(lsa_StringLarge_Type, py_message, return false;);
		if (talloc_reference(r, py_talloc_get_mem_ctx(py_message)) == NULL) {
			PyErr_NoMemory();
			return false;
		}
		r->in.message = (struct lsa_StringLarge *)py_talloc_get_ptr(py_message);
	}
	PY_CHECK_TYPE(&PyInt_Type, py_timeout, return false;);
	r->in.timeout = PyInt_AsLong(py_timeout);
	PY_CHECK_TYPE(&PyInt_Type, py_force_apps, return false;);
	r->in.force_apps = PyInt_AsLong(py_force_apps);
	PY_CHECK_TYPE(&PyInt_Type, py_do_reboot, return false;);
	r->in.do_reboot = PyInt_AsLong(py_do_reboot);
	PY_CHECK_TYPE(&PyInt_Type, py_reason, return false;);
	r->in.reason = PyInt_AsLong(py_reason);
	return true;
}

static PyObject *unpack_py_initshutdown_InitEx_args_out(struct initshutdown_InitEx *r)
{
	PyObject *result;
	result = Py_None;
	Py_INCREF(result);
	if (!W_ERROR_IS_OK(r->out.result)) {
		PyErr_SetWERROR(r->out.result);
		return NULL;
	}

	return result;
}

const struct PyNdrRpcMethodDef py_ndr_initshutdown_methods[] = {
	{ "Init", "S.Init(hostname, message, timeout, force_apps, do_reboot) -> None", (py_dcerpc_call_fn)dcerpc_initshutdown_Init_r, (py_data_pack_fn)pack_py_initshutdown_Init_args_in, (py_data_unpack_fn)unpack_py_initshutdown_Init_args_out, 0, &ndr_table_initshutdown },
	{ "Abort", "S.Abort(server) -> None", (py_dcerpc_call_fn)dcerpc_initshutdown_Abort_r, (py_data_pack_fn)pack_py_initshutdown_Abort_args_in, (py_data_unpack_fn)unpack_py_initshutdown_Abort_args_out, 1, &ndr_table_initshutdown },
	{ "InitEx", "S.InitEx(hostname, message, timeout, force_apps, do_reboot, reason) -> None", (py_dcerpc_call_fn)dcerpc_initshutdown_InitEx_r, (py_data_pack_fn)pack_py_initshutdown_InitEx_args_in, (py_data_unpack_fn)unpack_py_initshutdown_InitEx_args_out, 2, &ndr_table_initshutdown },
	{ NULL }
};

static PyObject *interface_initshutdown_new(PyTypeObject *type, PyObject *args, PyObject *kwargs)
{
	return py_dcerpc_interface_init_helper(type, args, kwargs, &ndr_table_initshutdown);
}

#define PY_DOC_INITSHUTDOWN "Init shutdown service"
static PyTypeObject initshutdown_InterfaceType = {
	PyObject_HEAD_INIT(NULL) 0,
	.tp_name = "initshutdown.initshutdown",
	.tp_basicsize = sizeof(dcerpc_InterfaceObject),
	.tp_doc = "initshutdown(binding, lp_ctx=None, credentials=None) -> connection\n"
"\n"
"binding should be a DCE/RPC binding string (for example: ncacn_ip_tcp:127.0.0.1)\n"
"lp_ctx should be a path to a smb.conf file or a param.LoadParm object\n"
"credentials should be a credentials.Credentials object.\n\n"PY_DOC_INITSHUTDOWN,
	.tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
	.tp_new = interface_initshutdown_new,
};

static PyMethodDef initshutdown_methods[] = {
	{ NULL, NULL, 0, NULL }
};

void initinitshutdown(void)
{
	PyObject *m;
	PyObject *dep_samba_dcerpc_lsa;
	PyObject *dep_samba_dcerpc_base;

	dep_samba_dcerpc_lsa = PyImport_ImportModule("samba.dcerpc.lsa");
	if (dep_samba_dcerpc_lsa == NULL)
		return;

	dep_samba_dcerpc_base = PyImport_ImportModule("samba.dcerpc.base");
	if (dep_samba_dcerpc_base == NULL)
		return;

	lsa_StringLarge_Type = (PyTypeObject *)PyObject_GetAttrString(dep_samba_dcerpc_lsa, "StringLarge");
	if (lsa_StringLarge_Type == NULL)
		return;

	ClientConnection_Type = (PyTypeObject *)PyObject_GetAttrString(dep_samba_dcerpc_base, "ClientConnection");
	if (ClientConnection_Type == NULL)
		return;

	initshutdown_InterfaceType.tp_base = ClientConnection_Type;

	if (PyType_Ready(&initshutdown_InterfaceType) < 0)
		return;
	if (!PyInterface_AddNdrRpcMethods(&initshutdown_InterfaceType, py_ndr_initshutdown_methods))
		return;

#ifdef PY_INITSHUTDOWN_PATCH
	PY_INITSHUTDOWN_PATCH(&initshutdown_InterfaceType);
#endif

	m = Py_InitModule3("initshutdown", initshutdown_methods, "initshutdown DCE/RPC");
	if (m == NULL)
		return;

	PyModule_AddObject(m, "SHTDN_REASON_MINOR_TERMSRV", PyInt_FromLong(SHTDN_REASON_MINOR_TERMSRV));
	PyModule_AddObject(m, "SHTDN_REASON_MINOR_HOTFIX", PyInt_FromLong(SHTDN_REASON_MINOR_HOTFIX));
	PyModule_AddObject(m, "SHTDN_REASON_MINOR_SERVICEPACK", PyInt_FromLong(SHTDN_REASON_MINOR_SERVICEPACK));
	PyModule_AddObject(m, "SHTDN_REASON_MINOR_RECONFIG", PyInt_FromLong(SHTDN_REASON_MINOR_RECONFIG));
	PyModule_AddObject(m, "SHTDN_REASON_MINOR_UNSTABLE", PyInt_FromLong(SHTDN_REASON_MINOR_UNSTABLE));
	PyModule_AddObject(m, "SHTDN_REASON_MINOR_PROCESSOR", PyInt_FromLong(SHTDN_REASON_MINOR_PROCESSOR));
	PyModule_AddObject(m, "SHTDN_REASON_MINOR_SERVICEPACK_UNINSTALL", PyInt_FromLong(SHTDN_REASON_MINOR_SERVICEPACK_UNINSTALL));
	PyModule_AddObject(m, "SHTDN_REASON_MINOR_INSTALLATION", PyInt_FromLong(SHTDN_REASON_MINOR_INSTALLATION));
	PyModule_AddObject(m, "SHTDN_REASON_MINOR_OTHER", PyInt_FromLong(SHTDN_REASON_MINOR_OTHER));
	PyModule_AddObject(m, "SHTDN_REASON_MAJOR_APPLICATION", PyInt_FromLong(SHTDN_REASON_MAJOR_APPLICATION));
	PyModule_AddObject(m, "SHTDN_REASON_MINOR_OTHERDRIVER", PyInt_FromLong(SHTDN_REASON_MINOR_OTHERDRIVER));
	PyModule_AddObject(m, "SHTDN_REASON_MAJOR_OTHER", PyInt_FromLong(SHTDN_REASON_MAJOR_OTHER));
	PyModule_AddObject(m, "SHTDN_REASON_MINOR_HUNG", PyInt_FromLong(SHTDN_REASON_MINOR_HUNG));
	PyModule_AddObject(m, "SHTDN_REASON_MINOR_UPGRADE", PyInt_FromLong(SHTDN_REASON_MINOR_UPGRADE));
	PyModule_AddObject(m, "SHTDN_REASON_MAJOR_SYSTEM", PyInt_FromLong(SHTDN_REASON_MAJOR_SYSTEM));
	PyModule_AddObject(m, "SHTDN_REASON_MINOR_DISK", PyInt_FromLong(SHTDN_REASON_MINOR_DISK));
	PyModule_AddObject(m, "SHTDN_REASON_FLAG_PLANNED", PyInt_FromLong(SHTDN_REASON_FLAG_PLANNED));
	PyModule_AddObject(m, "SHTDN_REASON_MINOR_CORDUNPLUGGED", PyInt_FromLong(SHTDN_REASON_MINOR_CORDUNPLUGGED));
	PyModule_AddObject(m, "SHTDN_REASON_MINOR_NETWORK_CONNECTIVITY", PyInt_FromLong(SHTDN_REASON_MINOR_NETWORK_CONNECTIVITY));
	PyModule_AddObject(m, "SHTDN_REASON_MINOR_MAINTENANCE", PyInt_FromLong(SHTDN_REASON_MINOR_MAINTENANCE));
	PyModule_AddObject(m, "SHTDN_REASON_MAJOR_SOFTWARE", PyInt_FromLong(SHTDN_REASON_MAJOR_SOFTWARE));
	PyModule_AddObject(m, "SHTDN_REASON_MINOR_BLUESCREEN", PyInt_FromLong(SHTDN_REASON_MINOR_BLUESCREEN));
	PyModule_AddObject(m, "SHTDN_REASON_MAJOR_HARDWARE", PyInt_FromLong(SHTDN_REASON_MAJOR_HARDWARE));
	PyModule_AddObject(m, "SHTDN_REASON_MINOR_ENVIRONMENT", PyInt_FromLong(SHTDN_REASON_MINOR_ENVIRONMENT));
	PyModule_AddObject(m, "SHTDN_REASON_MINOR_NETWORKCARD", PyInt_FromLong(SHTDN_REASON_MINOR_NETWORKCARD));
	PyModule_AddObject(m, "SHTDN_REASON_MINOR_MMC", PyInt_FromLong(SHTDN_REASON_MINOR_MMC));
	PyModule_AddObject(m, "SHTDN_REASON_MINOR_WMI", PyInt_FromLong(SHTDN_REASON_MINOR_WMI));
	PyModule_AddObject(m, "SHTDN_REASON_MINOR_POWER_SUPPLY", PyInt_FromLong(SHTDN_REASON_MINOR_POWER_SUPPLY));
	PyModule_AddObject(m, "SHTDN_REASON_MAJOR_OPERATINGSYSTEM", PyInt_FromLong(SHTDN_REASON_MAJOR_OPERATINGSYSTEM));
	PyModule_AddObject(m, "SHTDN_REASON_MAJOR_LEGACY_API", PyInt_FromLong(SHTDN_REASON_MAJOR_LEGACY_API));
	PyModule_AddObject(m, "SHTDN_REASON_MAJOR_POWER", PyInt_FromLong(SHTDN_REASON_MAJOR_POWER));
	PyModule_AddObject(m, "SHTDN_REASON_MINOR_HOTFIX_UNINSTALL", PyInt_FromLong(SHTDN_REASON_MINOR_HOTFIX_UNINSTALL));
	PyModule_AddObject(m, "SHTDN_REASON_MINOR_HARDWARE_DRIVER", PyInt_FromLong(SHTDN_REASON_MINOR_HARDWARE_DRIVER));
	PyModule_AddObject(m, "SHTDN_REASON_MINOR_SECURITYFIX_UNINSTALL", PyInt_FromLong(SHTDN_REASON_MINOR_SECURITYFIX_UNINSTALL));
	PyModule_AddObject(m, "SHTDN_REASON_FLAG_USER_DEFINED", PyInt_FromLong(SHTDN_REASON_FLAG_USER_DEFINED));
	PyModule_AddObject(m, "SHTDN_REASON_MINOR_SECURITYFIX", PyInt_FromLong(SHTDN_REASON_MINOR_SECURITYFIX));
	PyModule_AddObject(m, "SHTDN_REASON_MINOR_SECURITY", PyInt_FromLong(SHTDN_REASON_MINOR_SECURITY));
	Py_INCREF((PyObject *)(void *)&initshutdown_InterfaceType);
	PyModule_AddObject(m, "initshutdown", (PyObject *)(void *)&initshutdown_InterfaceType);
#ifdef PY_MOD_INITSHUTDOWN_PATCH
	PY_MOD_INITSHUTDOWN_PATCH(m);
#endif

}
