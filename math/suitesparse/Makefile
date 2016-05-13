# Created by: Maho Nakata <maho@FreeBSD.org>
# $FreeBSD$

PORTNAME=	suitesparse
PORTVERSION=	4.4.6
CATEGORIES=	math
MASTER_SITES=	http://faculty.cse.tamu.edu/davis/SuiteSparse/
DISTNAME=	SuiteSparse-${PORTVERSION}

MAINTAINER=	maho@FreeBSD.org
COMMENT=	SuiteSparse is a set of packages for sparse matrices calculation

LICENSE=	GPLv2 LGPL21
LICENSE_COMB=	multi

USES=		fortran gmake

OPTIONS_RADIO=	BLAS
OPTIONS_RADIO_BLAS=	OPENBLAS NETLIB ATLAS
OPTIONS_DEFAULT=	OPENBLAS

OPENBLAS_USES=	blaslapack:openblas
NETLIB_USES=	blaslapack:netlib
ATLAS_USES=	blaslapack:atlas

.if defined (WITH_METIS)
LIB_DEPENDS=	libmetis.so:math/metis4

RESTRICTED=	GPL and METIS licenses are contradictory
.endif

CONFLICTS=	umfpack-*

LDFLAGS+=	-L${LOCALBASE}/lib

.include <bsd.port.options.mk>

.if ${ARCH} == "sparc64"
PICFLAG?=	-fPIC
.else
PICFLAG?=	-fpic
.endif

USE_LDCONFIG=	yes
WRKSRC=${WRKDIR}/SuiteSparse
WRKSRC_SHARED=${WRKSRC}_shared
SVERSION=1
# random failures in multi-cpu build reported upstream
MAKE_JOBS_NUMBER=1

.if defined (WITH_METIS)
METIS=${LOCALBASE}/lib/libmetis.a
METIS_PATH=${LOCALBASE}/include/metis
CHOLMOD_CONFIG=
.else
CHOLMOD_CONFIG=		-DNPARTITION
.endif

ALL_STATIC_LIBS+=SuiteSparse_config/libsuitesparseconfig.a
ALL_STATIC_LIBS+=AMD/Lib/libamd.a
ALL_STATIC_LIBS+=BTF/Lib/libbtf.a
ALL_STATIC_LIBS+=CAMD/Lib/libcamd.a
ALL_STATIC_LIBS+=COLAMD/Lib/libcolamd.a
ALL_STATIC_LIBS+=CCOLAMD/Lib/libccolamd.a
ALL_STATIC_LIBS+=CHOLMOD/Lib/libcholmod.a
ALL_STATIC_LIBS+=CSparse/Lib/libcsparse.a
ALL_STATIC_LIBS+=CXSparse/Lib/libcxsparse.a
ALL_STATIC_LIBS+=KLU/Lib/libklu.a
ALL_STATIC_LIBS+=LDL/Lib/libldl.a
ALL_STATIC_LIBS+=RBio/Lib/librbio.a
ALL_STATIC_LIBS+=SPQR/Lib/libspqr.a
ALL_STATIC_LIBS+=SuiteSparse_config/xerbla/libcerbla.a
ALL_STATIC_LIBS+=UMFPACK/Lib/libumfpack.a
OURLIBS=-L${WRKSRC_SHARED}/lib
MISSING_DEPS_suitesparseconfig=	-lm
MISSING_DEPS_amd=		${OURLIBS} -lsuitesparseconfig -lm
MISSING_DEPS_camd=		${OURLIBS} -lsuitesparseconfig -lm
MISSING_DEPS_colamd=		${OURLIBS} -lsuitesparseconfig -lm
MISSING_DEPS_ccolamd=		${OURLIBS} -lsuitesparseconfig -lm
MISSING_DEPS_cholmod=		-L${LOCALBASE}/lib ${LAPACKLIB} ${OURLIBS} -lamd -lcamd -lcolamd -lccolamd -lsuitesparseconfig -lm
MISSING_DEPS_klu=		${OURLIBS} -lamd -lbtf -lsuitesparseconfig -lm
MISSING_DEPS_spqr=		-L${LOCALBASE}/lib ${LAPACKLIB} ${OURLIBS} -lcholmod -lsuitesparseconfig -lm
MISSING_DEPS_umfpack=		-L${LOCALBASE}/lib ${LAPACKLIB} ${OURLIBS} -lamd -lcholmod -lsuitesparseconfig -lm

post-patch:
	@${FIND} -X ${WRKSRC} -type f -name 'Makefile' -or -name '*.mk' | ${XARGS} \
	${REINPLACE_CMD} -i '' -E \
			-e '\|^[[:space:]]*AR =|s|ar|${AR}|' \
			-e '\|^[[:space:]]*BLAS =|s|=.*|= ${LDFLAGS} ${BLASLIB}|' \
			-e '\|^[[:space:]]*CC =|d' \
			-e '\|^[[:space:]]*CFLAGS =|{s|-O[1-9]\{0,1\}||g;s|=|= ${CFLAGS} $${PICFLAG} |;}' \
			-e '\|^[[:space:]]*CHOLMOD_CONFIG =|s|=.*|= ${CHOLMOD_CONFIG}|' \
			-e '\|^[[:space:]]*CPLUSPLUS =|s|=.*|= ${CXX}|' \
			-e '\|^[[:space:]]*F77 =|d' \
			-e '\|^[[:space:]]*F77FLAGS =|s|=.*|= ${FFLAGS} $${PICFLAG}|' \
			-e '\|^[[:space:]]*LAPACK =|s|=.*|= ${LDFLAGS} ${LAPACKLIB}|' \
			-e '\|^[[:space:]]*METIS =|s|=.*|= ${METIS}|' \
			-e '\|^[[:space:]]*METIS_PATH =|s|=.*|= ${METIS_PATH}|' \
			-e 's|-I\$$(METIS_PATH)/Lib|-I${METIS_PATH}|' \
			-e '\|^[[:space:]]*RANLIB =|s|ranlib|${RANLIB}|' \
			-e '\|^[[:space:]]*SPQR_CONFIG =|s|=.*|= ${CHOLMOD_CONFIG}|' \
			-e 's|# MAKE = gmake|MAKE = gmake|g' \
			-e 's|/usr/local|${LOCALBASE}|g'
	@${CP} -r ${WRKSRC} ${WRKSRC_SHARED}
	@${FIND} -X ${WRKSRC_SHARED} -type f -name 'Makefile' -or -name '*.mk' | ${XARGS} \
	${REINPLACE_CMD} -e 's|$$(CFLAGS)|$$(CFLAGS) $$(PICFLAG)|g'

do-build:
	@cd ${WRKSRC} ; ${SETENV} ${MAKE_ENV} PICFLAG= \
		${MAKE_CMD} ${_MAKE_JOBS} ${MAKE_ARGS}
	@cd ${WRKSRC_SHARED} ; ${SETENV} ${MAKE_ENV} PICFLAG=${PICFLAG} \
		${MAKE_CMD} ${_MAKE_JOBS} ${MAKE_ARGS}

post-build:
	@${MKDIR} ${WRKSRC_SHARED}/lib
.for i in ${ALL_STATIC_LIBS}
	@${LD} -shared -o ${WRKSRC_SHARED}/lib/${i:C/.*\///:C/.a$//}.so.${SVERSION} -soname ${i:C/.*\///:C/.a$//}.so.${SVERSION} --whole-archive ${WRKSRC_SHARED}/${i} \
		${MISSING_DEPS_${i:C/.*\///:C/.a$//:C/^lib//}}
	@cd ${WRKSRC_SHARED}/lib && ${LN} -s ${i:C/.*\///:C/.a$//}.so.${SVERSION} ${i:C/.*\///:C/.a$//}.so
.endfor


do-install:
	@${MKDIR} ${STAGEDIR}${PREFIX}/include/suitesparse
	@${LN} -sf ${PREFIX}/include/suitesparse ${STAGEDIR}${PREFIX}/include/ufsparse
.for d in AMD BTF CAMD CCOLAMD CHOLMOD COLAMD CXSparse KLU SPQR UMFPACK
	@${INSTALL_DATA} ${WRKSRC}/${d}/Include/*.h ${STAGEDIR}${PREFIX}/include/suitesparse
.endfor
	@${INSTALL_DATA} ${WRKSRC}/SuiteSparse_config/SuiteSparse_config.h ${STAGEDIR}${PREFIX}/include/suitesparse
	@${INSTALL_DATA} ${WRKSRC}/UMFPACK/Source/umf_config.h ${STAGEDIR}${PREFIX}/include/suitesparse
	@${INSTALL_DATA} ${WRKSRC}/SPQR/Include/*.hpp ${STAGEDIR}${PREFIX}/include/suitesparse
	@${RM} -rf ${STAGEDIR}${PREFIX}/lib
	@${CP} -RP ${WRKSRC_SHARED}/lib ${STAGEDIR}${PREFIX}/lib
	@${CP} `find ${WRKSRC} -name "*.a"` ${STAGEDIR}${PREFIX}/lib/
	@${STRIP_CMD} ${STAGEDIR}${PREFIX}/lib/*.so.1

.include <bsd.port.mk>
