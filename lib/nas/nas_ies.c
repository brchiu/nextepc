/*
 * Copyright (c) 2017, CellWire Group
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.

 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*******************************************************************************
 * This file had been created by gtpv2c_tlv.py script v0.1.0
 * Please do not modify this file but regenerate it via script.
 * Created on: 2017-04-13 13:13:49.598913 by acetcom
 * from 24301-d80.docx
 ******************************************************************************/

#define TRACE_MODULE _nasies

#include "core_debug.h"
#include "core_lib.h"
#include "nas_ies.h"

c_int16_t nas_encode_optional_type(pkbuf_t *pkbuf, c_uint8_t type)
{
    c_uint16_t size = sizeof(c_uint8_t);

    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(pkbuf->payload - size, &type, size);

    return size;
}
/* 9.9.2.0A Device properties
 * O TV 1 */
c_int16_t nas_decode_device_properties(nas_device_properties_t *device_properties, pkbuf_t *pkbuf)
{
    memcpy(device_properties, pkbuf->payload - 1, 1);

    return 0;
}

c_int16_t nas_encode_device_properties(pkbuf_t *pkbuf, nas_device_properties_t *device_properties)
{
    c_uint16_t size = sizeof(nas_device_properties_t);

    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(pkbuf->payload - size, device_properties, size);

    return size;
}

/* 9.9.2.10 Supported Codec List
 * O TLV 5-n */
c_int16_t nas_decode_supported_codec_list(nas_supported_codec_list_t *supported_codec_list, pkbuf_t *pkbuf)
{
    c_uint16_t size = 0;
    nas_supported_codec_list_t *source = pkbuf->payload;

    supported_codec_list->length = source->length;
    size = supported_codec_list->length + sizeof(supported_codec_list->length);

    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(supported_codec_list, pkbuf->payload - size, size);

    return size;
}

c_int16_t nas_encode_supported_codec_list(pkbuf_t *pkbuf, nas_supported_codec_list_t *supported_codec_list)
{
    c_uint16_t size = supported_codec_list->length + sizeof(supported_codec_list->length);
    nas_supported_codec_list_t target;

    memcpy(&target, supported_codec_list, sizeof(nas_supported_codec_list_t));
    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(pkbuf->payload - size, &target, size);

    return size;
}

/* 9.9.2.2 Location area identification
 * O TV 6 */
c_int16_t nas_decode_location_area_identification(nas_location_area_identification_t *location_area_identification, pkbuf_t *pkbuf)
{
    c_uint16_t size = sizeof(nas_location_area_identification_t);

    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(location_area_identification, pkbuf->payload - size, size);

    location_area_identification->lac = ntohs(location_area_identification->lac);

    return size;
}

c_int16_t nas_encode_location_area_identification(pkbuf_t *pkbuf, nas_location_area_identification_t *location_area_identification)
{
    c_uint16_t size = sizeof(nas_location_area_identification_t);
    nas_location_area_identification_t target;

    memcpy(&target, location_area_identification, size);
    target.lac = htons(location_area_identification->lac);

    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(pkbuf->payload - size, &target, size);

    return size;
}

/* 9.9.2.3 Mobile identity
 * O TLV 7-10 */
c_int16_t nas_decode_mobile_identity(nas_mobile_identity_t *mobile_identity, pkbuf_t *pkbuf)
{
    c_uint16_t size = 0;
    nas_mobile_identity_t *source = pkbuf->payload;

    mobile_identity->length = source->length;
    size = mobile_identity->length + sizeof(mobile_identity->length);

    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(mobile_identity, pkbuf->payload - size, size);

    if (mobile_identity->tmsi.type == NAS_MOBILE_IDENTITY_TMSI)
    {
        if (mobile_identity->tmsi.spare != 0xf)
            d_warn("Spec warning : mobile_identity->tmsi.spare = 0x%x", mobile_identity->tmsi.spare);
        mobile_identity->tmsi.tmsi = ntohl(mobile_identity->tmsi.tmsi);
    }

    return size;
}

c_int16_t nas_encode_mobile_identity(pkbuf_t *pkbuf, nas_mobile_identity_t *mobile_identity)
{
    c_uint16_t size = mobile_identity->length + sizeof(mobile_identity->length);
    nas_mobile_identity_t target;

    memcpy(&target, mobile_identity, sizeof(nas_mobile_identity_t));
    if (mobile_identity->tmsi.type == NAS_MOBILE_IDENTITY_TMSI)
    {
        target.tmsi.tmsi = htonl(mobile_identity->tmsi.tmsi);
        target.tmsi.spare = 0xf;
    }

    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(pkbuf->payload - size, &target, size);

    return size;
}

/* 9.9.2.4 Mobile station classmark 2
 * O TLV 5 */
c_int16_t nas_decode_mobile_station_classmark_2(nas_mobile_station_classmark_2_t *mobile_station_classmark_2, pkbuf_t *pkbuf)
{
    c_uint16_t size = 0;
    nas_mobile_station_classmark_2_t *source = pkbuf->payload;

    mobile_station_classmark_2->length = source->length;
    size = mobile_station_classmark_2->length + sizeof(mobile_station_classmark_2->length);

    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(mobile_station_classmark_2, pkbuf->payload - size, size);

    return size;
}

c_int16_t nas_encode_mobile_station_classmark_2(pkbuf_t *pkbuf, nas_mobile_station_classmark_2_t *mobile_station_classmark_2)
{
    c_uint16_t size = mobile_station_classmark_2->length + sizeof(mobile_station_classmark_2->length);
    nas_mobile_station_classmark_2_t target;

    memcpy(&target, mobile_station_classmark_2, sizeof(nas_mobile_station_classmark_2_t));
    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(pkbuf->payload - size, &target, size);

    return size;
}

/* 9.9.2.5 Mobile station classmark 3
 * O TLV 2-34 */
c_int16_t nas_decode_mobile_station_classmark_3(nas_mobile_station_classmark_3_t *mobile_station_classmark_3, pkbuf_t *pkbuf)
{
    c_uint16_t size = 0;
    nas_mobile_station_classmark_3_t *source = pkbuf->payload;

    mobile_station_classmark_3->length = source->length;
    size = mobile_station_classmark_3->length + sizeof(mobile_station_classmark_3->length);

    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(mobile_station_classmark_3, pkbuf->payload - size, size);

    return size;
}

c_int16_t nas_encode_mobile_station_classmark_3(pkbuf_t *pkbuf, nas_mobile_station_classmark_3_t *mobile_station_classmark_3)
{
    c_uint16_t size = mobile_station_classmark_3->length + sizeof(mobile_station_classmark_3->length);
    nas_mobile_station_classmark_3_t target;

    memcpy(&target, mobile_station_classmark_3, sizeof(nas_mobile_station_classmark_3_t));
    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(pkbuf->payload - size, &target, size);

    return size;
}

/* 9.9.2.8 PLMN list
 * O TLV 5-47 */
c_int16_t nas_decode_plmn_list(nas_plmn_list_t *plmn_list, pkbuf_t *pkbuf)
{
    c_uint16_t size = 0;
    nas_plmn_list_t *source = pkbuf->payload;

    plmn_list->length = source->length;
    size = plmn_list->length + sizeof(plmn_list->length);

    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(plmn_list, pkbuf->payload - size, size);

    return size;
}

c_int16_t nas_encode_plmn_list(pkbuf_t *pkbuf, nas_plmn_list_t *plmn_list)
{
    c_uint16_t size = plmn_list->length + sizeof(plmn_list->length);
    nas_plmn_list_t target;

    memcpy(&target, plmn_list, sizeof(nas_plmn_list_t));
    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(pkbuf->payload - size, &target, size);

    return size;
}

/* 9.9.3.0A Additional update result
 * O TV 1 */
c_int16_t nas_decode_additional_update_result(nas_additional_update_result_t *additional_update_result, pkbuf_t *pkbuf)
{
    memcpy(additional_update_result, pkbuf->payload - 1, 1);

    return 0;
}

c_int16_t nas_encode_additional_update_result(pkbuf_t *pkbuf, nas_additional_update_result_t *additional_update_result)
{
    c_uint16_t size = sizeof(nas_additional_update_result_t);

    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(pkbuf->payload - size, additional_update_result, size);

    return size;
}

/* 9.9.3.0B Additional update type
 * O TV 1 */
c_int16_t nas_decode_additional_update_type(nas_additional_update_type_t *additional_update_type, pkbuf_t *pkbuf)
{
    memcpy(additional_update_type, pkbuf->payload - 1, 1);

    return 0;
}

c_int16_t nas_encode_additional_update_type(pkbuf_t *pkbuf, nas_additional_update_type_t *additional_update_type)
{
    c_uint16_t size = sizeof(nas_additional_update_type_t);

    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(pkbuf->payload - size, additional_update_type, size);

    return size;
}

/* 9.9.3.1 Authentication failure parameter
 * O TLV 16 */
c_int16_t nas_decode_authentication_failure_parameter(nas_authentication_failure_parameter_t *authentication_failure_parameter, pkbuf_t *pkbuf)
{
    c_uint16_t size = 0;
    nas_authentication_failure_parameter_t *source = pkbuf->payload;

    authentication_failure_parameter->length = source->length;
    size = authentication_failure_parameter->length + sizeof(authentication_failure_parameter->length);

    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(authentication_failure_parameter, pkbuf->payload - size, size);

    return size;
}

c_int16_t nas_encode_authentication_failure_parameter(pkbuf_t *pkbuf, nas_authentication_failure_parameter_t *authentication_failure_parameter)
{
    c_uint16_t size = authentication_failure_parameter->length + sizeof(authentication_failure_parameter->length);
    nas_authentication_failure_parameter_t target;

    memcpy(&target, authentication_failure_parameter, sizeof(nas_authentication_failure_parameter_t));
    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(pkbuf->payload - size, &target, size);

    return size;
}

/* 9.9.3.10 EPS attach result
 * M V 1/2 */
c_int16_t nas_decode_eps_attach_result(nas_eps_attach_result_t *eps_attach_result, pkbuf_t *pkbuf)
{
    c_uint16_t size = sizeof(nas_eps_attach_result_t);

    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(eps_attach_result, pkbuf->payload - size, size);

    return size;
}

c_int16_t nas_encode_eps_attach_result(pkbuf_t *pkbuf, nas_eps_attach_result_t *eps_attach_result)
{
    c_uint16_t size = sizeof(nas_eps_attach_result_t);
    nas_eps_attach_result_t target;

    memcpy(&target, eps_attach_result, size);
    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(pkbuf->payload - size, &target, size);

    return size;
}

/* 9.9.3.11 EPS attach type
 * M V 1/2 */
c_int16_t nas_decode_eps_attach_type(nas_eps_attach_type_t *eps_attach_type, pkbuf_t *pkbuf)
{
    c_uint16_t size = sizeof(nas_eps_attach_type_t);

    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(eps_attach_type, pkbuf->payload - size, size);

    return size;
}

c_int16_t nas_encode_eps_attach_type(pkbuf_t *pkbuf, nas_eps_attach_type_t *eps_attach_type)
{
    c_uint16_t size = sizeof(nas_eps_attach_type_t);
    nas_eps_attach_type_t target;

    memcpy(&target, eps_attach_type, size);
    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(pkbuf->payload - size, &target, size);

    return size;
}

/* 9.9.3.12 EPS mobile identity
 * M LV 5-12 */
c_int16_t nas_decode_eps_mobile_identity(nas_eps_mobile_identity_t *eps_mobile_identity, pkbuf_t *pkbuf)
{
    c_uint16_t size = 0;
    nas_eps_mobile_identity_t *source = pkbuf->payload;

    eps_mobile_identity->length = source->length;
    size = eps_mobile_identity->length + sizeof(eps_mobile_identity->length);

    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(eps_mobile_identity, pkbuf->payload - size, size);

    if (eps_mobile_identity->guti.type == NAS_EPS_MOBILE_IDENTITY_GUTI)
    {
        if (eps_mobile_identity->guti.spare != 0xf)
            d_warn("Spec warning : eps_mobile_identy->spare = 0x%x", eps_mobile_identity->guti.spare);
        eps_mobile_identity->guti.mme_gid = ntohs(eps_mobile_identity->guti.mme_gid);
        eps_mobile_identity->guti.m_tmsi = ntohl(eps_mobile_identity->guti.m_tmsi);
    }

    return size;
}

c_int16_t nas_encode_eps_mobile_identity(pkbuf_t *pkbuf, nas_eps_mobile_identity_t *eps_mobile_identity)
{
    c_uint16_t size = eps_mobile_identity->length + sizeof(eps_mobile_identity->length);
    nas_eps_mobile_identity_t target;

    memcpy(&target, eps_mobile_identity, sizeof(nas_eps_mobile_identity_t));
    if (target.guti.type == NAS_EPS_MOBILE_IDENTITY_GUTI)
    {
        target.guti.spare = 0xf;
        target.guti.mme_gid = htons(eps_mobile_identity->guti.mme_gid);
        target.guti.m_tmsi = htonl(eps_mobile_identity->guti.m_tmsi);
    }

    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(pkbuf->payload - size, &target, size);

    return size;
}

/* 9.9.3.12A EPS network feature support
 * O TLV 3 */
c_int16_t nas_decode_eps_network_feature_support(nas_eps_network_feature_support_t *eps_network_feature_support, pkbuf_t *pkbuf)
{
    c_uint16_t size = 0;
    nas_eps_network_feature_support_t *source = pkbuf->payload;

    eps_network_feature_support->length = source->length;
    size = eps_network_feature_support->length + sizeof(eps_network_feature_support->length);

    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(eps_network_feature_support, pkbuf->payload - size, size);

    return size;
}

c_int16_t nas_encode_eps_network_feature_support(pkbuf_t *pkbuf, nas_eps_network_feature_support_t *eps_network_feature_support)
{
    c_uint16_t size = eps_network_feature_support->length + sizeof(eps_network_feature_support->length);
    nas_eps_network_feature_support_t target;

    memcpy(&target, eps_network_feature_support, sizeof(nas_eps_network_feature_support_t));
    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(pkbuf->payload - size, &target, size);

    return size;
}

/* 9.9.3.15 ESM message container
 * M LV-E 5-n */
c_int16_t nas_decode_esm_message_container(nas_esm_message_container_t *esm_message_container, pkbuf_t *pkbuf)
{
    c_uint16_t size = 0;
    nas_esm_message_container_t *source = pkbuf->payload;

    esm_message_container->len = ntohs(source->len);
    size = esm_message_container->len + sizeof(esm_message_container->len);

    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    esm_message_container->data = pkbuf->payload - size + sizeof(esm_message_container->len);

    return size;
}

c_int16_t nas_encode_esm_message_container(pkbuf_t *pkbuf, nas_esm_message_container_t *esm_message_container)
{
    c_uint16_t size = 0;
    c_uint16_t target;

    d_assert(esm_message_container, return -1, "Null param");
    d_assert(esm_message_container->data, return -1, "Null param");

    size = sizeof(esm_message_container->len);
    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    target = htons(esm_message_container->len);
    memcpy(pkbuf->payload - size, &target, size);

    size = esm_message_container->len;
    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(pkbuf->payload - size, esm_message_container->data, size);

    return esm_message_container->len + sizeof(esm_message_container->len);
}

/* 9.9.3.16 GPRS timer
 * M V 1 */
c_int16_t nas_decode_gprs_timer(nas_gprs_timer_t *gprs_timer, pkbuf_t *pkbuf)
{
    c_uint16_t size = sizeof(nas_gprs_timer_t);

    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(gprs_timer, pkbuf->payload - size, size);

    return size;
}

c_int16_t nas_encode_gprs_timer(pkbuf_t *pkbuf, nas_gprs_timer_t *gprs_timer)
{
    c_uint16_t size = sizeof(nas_gprs_timer_t);
    nas_gprs_timer_t target;

    memcpy(&target, gprs_timer, size);
    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(pkbuf->payload - size, &target, size);

    return size;
}

/* 9.9.3.16A GPRS timer 2
 * O TLV 3 */
c_int16_t nas_decode_gprs_timer_2(nas_gprs_timer_2_t *gprs_timer_2, pkbuf_t *pkbuf)
{
    c_uint16_t size = 0;
    nas_gprs_timer_2_t *source = pkbuf->payload;

    gprs_timer_2->length = source->length;
    size = gprs_timer_2->length + sizeof(gprs_timer_2->length);

    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(gprs_timer_2, pkbuf->payload - size, size);

    return size;
}

c_int16_t nas_encode_gprs_timer_2(pkbuf_t *pkbuf, nas_gprs_timer_2_t *gprs_timer_2)
{
    c_uint16_t size = gprs_timer_2->length + sizeof(gprs_timer_2->length);
    nas_gprs_timer_2_t target;

    memcpy(&target, gprs_timer_2, sizeof(nas_gprs_timer_2_t));
    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(pkbuf->payload - size, &target, size);

    return size;
}

/* 9.9.3.16B GPRS timer 3
 * O TLV 3 */
c_int16_t nas_decode_gprs_timer_3(nas_gprs_timer_3_t *gprs_timer_3, pkbuf_t *pkbuf)
{
    c_uint16_t size = 0;
    nas_gprs_timer_3_t *source = pkbuf->payload;

    gprs_timer_3->length = source->length;
    size = gprs_timer_3->length + sizeof(gprs_timer_3->length);

    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(gprs_timer_3, pkbuf->payload - size, size);

    return size;
}

c_int16_t nas_encode_gprs_timer_3(pkbuf_t *pkbuf, nas_gprs_timer_3_t *gprs_timer_3)
{
    c_uint16_t size = gprs_timer_3->length + sizeof(gprs_timer_3->length);
    nas_gprs_timer_3_t target;

    memcpy(&target, gprs_timer_3, sizeof(nas_gprs_timer_3_t));
    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(pkbuf->payload - size, &target, size);

    return size;
}

/* 9.9.3.17 Identity type 2
 * M V 1/2 */
c_int16_t nas_decode_identity_type_2(nas_identity_type_2_t *identity_type_2, pkbuf_t *pkbuf)
{
    c_uint16_t size = sizeof(nas_identity_type_2_t);

    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(identity_type_2, pkbuf->payload - size, size);

    return size;
}

c_int16_t nas_encode_identity_type_2(pkbuf_t *pkbuf, nas_identity_type_2_t *identity_type_2)
{
    c_uint16_t size = sizeof(nas_identity_type_2_t);
    nas_identity_type_2_t target;

    memcpy(&target, identity_type_2, size);
    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(pkbuf->payload - size, &target, size);

    return size;
}

/* 9.9.3.18 IMEISV request
 * O TV 1 */
c_int16_t nas_decode_imeisv_request(nas_imeisv_request_t *imeisv_request, pkbuf_t *pkbuf)
{
    memcpy(imeisv_request, pkbuf->payload - 1, 1);

    return 0;
}

c_int16_t nas_encode_imeisv_request(pkbuf_t *pkbuf, nas_imeisv_request_t *imeisv_request)
{
    c_uint16_t size = sizeof(nas_imeisv_request_t);

    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(pkbuf->payload - size, imeisv_request, size);

    return size;
}

/* 9.9.3.2 Authentication parameter AUTN
 * M LV 17 */
c_int16_t nas_decode_authentication_parameter_autn(nas_authentication_parameter_autn_t *authentication_parameter_autn, pkbuf_t *pkbuf)
{
    c_uint16_t size = 0;
    nas_authentication_parameter_autn_t *source = pkbuf->payload;

    authentication_parameter_autn->length = source->length;
    size = authentication_parameter_autn->length + sizeof(authentication_parameter_autn->length);

    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(authentication_parameter_autn, pkbuf->payload - size, size);

    return size;
}

c_int16_t nas_encode_authentication_parameter_autn(pkbuf_t *pkbuf, nas_authentication_parameter_autn_t *authentication_parameter_autn)
{
    c_uint16_t size = authentication_parameter_autn->length + sizeof(authentication_parameter_autn->length);
    nas_authentication_parameter_autn_t target;

    memcpy(&target, authentication_parameter_autn, sizeof(nas_authentication_parameter_autn_t));
    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(pkbuf->payload - size, &target, size);

    return size;
}

/* 9.9.3.20 MS network capability
 * O TLV 4-10 */
c_int16_t nas_decode_ms_network_capability(nas_ms_network_capability_t *ms_network_capability, pkbuf_t *pkbuf)
{
    c_uint16_t size = 0;
    nas_ms_network_capability_t *source = pkbuf->payload;

    ms_network_capability->length = source->length;
    size = ms_network_capability->length + sizeof(ms_network_capability->length);

    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(ms_network_capability, pkbuf->payload - size, size);

    return size;
}

c_int16_t nas_encode_ms_network_capability(pkbuf_t *pkbuf, nas_ms_network_capability_t *ms_network_capability)
{
    c_uint16_t size = ms_network_capability->length + sizeof(ms_network_capability->length);
    nas_ms_network_capability_t target;

    memcpy(&target, ms_network_capability, sizeof(nas_ms_network_capability_t));
    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(pkbuf->payload - size, &target, size);

    return size;
}

/* 9.9.3.20A MS network feature support
 * O TV 1 */
c_int16_t nas_decode_ms_network_feature_support(nas_ms_network_feature_support_t *ms_network_feature_support, pkbuf_t *pkbuf)
{
    memcpy(ms_network_feature_support, pkbuf->payload - 1, 1);

    return 0;
}

c_int16_t nas_encode_ms_network_feature_support(pkbuf_t *pkbuf, nas_ms_network_feature_support_t *ms_network_feature_support)
{
    c_uint16_t size = sizeof(nas_ms_network_feature_support_t);

    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(pkbuf->payload - size, ms_network_feature_support, size);

    return size;
}

/* 9.9.3.21 key set identifier
 * M V 1/2 */
c_int16_t nas_decode_key_set_identifier(nas_key_set_identifier_t *key_set_identifier, pkbuf_t *pkbuf)
{
    c_uint16_t size = sizeof(nas_key_set_identifier_t);

    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(key_set_identifier, pkbuf->payload - size, size);

    return size;
}

c_int16_t nas_encode_key_set_identifier(pkbuf_t *pkbuf, nas_key_set_identifier_t *key_set_identifier)
{
    c_uint16_t size = sizeof(nas_key_set_identifier_t);
    nas_key_set_identifier_t target;

    memcpy(&target, key_set_identifier, size);
    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(pkbuf->payload - size, &target, size);

    return size;
}

/* 9.9.3.23 security algorithms
 * M V 1 */
c_int16_t nas_decode_security_algorithms(nas_security_algorithms_t *security_algorithms, pkbuf_t *pkbuf)
{
    c_uint16_t size = sizeof(nas_security_algorithms_t);

    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(security_algorithms, pkbuf->payload - size, size);

    return size;
}

c_int16_t nas_encode_security_algorithms(pkbuf_t *pkbuf, nas_security_algorithms_t *security_algorithms)
{
    c_uint16_t size = sizeof(nas_security_algorithms_t);
    nas_security_algorithms_t target;

    memcpy(&target, security_algorithms, size);
    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(pkbuf->payload - size, &target, size);

    return size;
}

/* 9.9.3.24A Network resource identifier container
 * O TLV 4 */
c_int16_t nas_decode_network_resource_identifier_container(nas_network_resource_identifier_container_t *network_resource_identifier_container, pkbuf_t *pkbuf)
{
    c_uint16_t size = 0;
    nas_network_resource_identifier_container_t *source = pkbuf->payload;

    network_resource_identifier_container->length = source->length;
    size = network_resource_identifier_container->length + sizeof(network_resource_identifier_container->length);

    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(network_resource_identifier_container, pkbuf->payload - size, size);

    return size;
}

c_int16_t nas_encode_network_resource_identifier_container(pkbuf_t *pkbuf, nas_network_resource_identifier_container_t *network_resource_identifier_container)
{
    c_uint16_t size = network_resource_identifier_container->length + sizeof(network_resource_identifier_container->length);
    nas_network_resource_identifier_container_t target;

    memcpy(&target, network_resource_identifier_container, sizeof(nas_network_resource_identifier_container_t));
    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(pkbuf->payload - size, &target, size);

    return size;
}

/* 9.9.3.25 Nonce
 * O TV 5 */
c_int16_t nas_decode_nonce(nas_nonce_t *nonce, pkbuf_t *pkbuf)
{
    c_uint16_t size = sizeof(nas_nonce_t);

    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(nonce, pkbuf->payload - size, size);

    *nonce = ntohl(*nonce);

    return size;
}

c_int16_t nas_encode_nonce(pkbuf_t *pkbuf, nas_nonce_t *nonce)
{
    c_uint16_t size = sizeof(nas_nonce_t);
    nas_nonce_t target;

    memcpy(&target, nonce, size);
    target = htonl(*nonce);

    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(pkbuf->payload - size, &target, size);

    return size;
}

/* 9.9.3.26 P-TMSI signature
 * O TV 4 */
c_int16_t nas_decode_p_tmsi_signature(nas_p_tmsi_signature_t *p_tmsi_signature, pkbuf_t *pkbuf)
{
    c_uint16_t size = 3;

    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(p_tmsi_signature, pkbuf->payload - size, size);

    *p_tmsi_signature = htonl(*p_tmsi_signature);

    return size;
}

c_int16_t nas_encode_p_tmsi_signature(pkbuf_t *pkbuf, nas_p_tmsi_signature_t *p_tmsi_signature)
{
    c_uint16_t size = 3;
    nas_p_tmsi_signature_t target;

    memcpy(&target, p_tmsi_signature, size);
    *p_tmsi_signature = ntohl(*p_tmsi_signature);

    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(pkbuf->payload - size, &target, size);

    return size;
}

/* 9.9.3.26A Extended EMM cause
 * O TV 1 */
c_int16_t nas_decode_extended_emm_cause(nas_extended_emm_cause_t *extended_emm_cause, pkbuf_t *pkbuf)
{
    memcpy(extended_emm_cause, pkbuf->payload - 1, 1);

    return 0;
}

c_int16_t nas_encode_extended_emm_cause(pkbuf_t *pkbuf, nas_extended_emm_cause_t *extended_emm_cause)
{
    c_uint16_t size = sizeof(nas_extended_emm_cause_t);

    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(pkbuf->payload - size, extended_emm_cause, size);

    return size;
}

/* 9.9.3.3 Authentication parameter RAND
 * M V 16 */
c_int16_t nas_decode_authentication_parameter_rand(nas_authentication_parameter_rand_t *authentication_parameter_rand, pkbuf_t *pkbuf)
{
    c_uint16_t size = sizeof(nas_authentication_parameter_rand_t);

    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(authentication_parameter_rand, pkbuf->payload - size, size);

    return size;
}

c_int16_t nas_encode_authentication_parameter_rand(pkbuf_t *pkbuf, nas_authentication_parameter_rand_t *authentication_parameter_rand)
{
    c_uint16_t size = sizeof(nas_authentication_parameter_rand_t);
    nas_authentication_parameter_rand_t target;

    memcpy(&target, authentication_parameter_rand, size);
    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(pkbuf->payload - size, &target, size);

    return size;
}

/* 9.9.3.31 TMSI status
 * O TV 1 */
c_int16_t nas_decode_tmsi_status(nas_tmsi_status_t *tmsi_status, pkbuf_t *pkbuf)
{
    memcpy(tmsi_status, pkbuf->payload - 1, 1);

    return 0;
}

c_int16_t nas_encode_tmsi_status(pkbuf_t *pkbuf, nas_tmsi_status_t *tmsi_status)
{
    c_uint16_t size = sizeof(nas_tmsi_status_t);

    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(pkbuf->payload - size, tmsi_status, size);

    return size;
}

/* 9.9.3.32 Tracking area identity
 * O TV 6 */
c_int16_t nas_decode_tracking_area_identity(nas_tracking_area_identity_t *tracking_area_identity, pkbuf_t *pkbuf)
{
    c_uint16_t size = sizeof(nas_tracking_area_identity_t);

    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(tracking_area_identity, pkbuf->payload - size, size);

    tracking_area_identity->tac = ntohs(tracking_area_identity->tac);

    return size;
}

c_int16_t nas_encode_tracking_area_identity(pkbuf_t *pkbuf, nas_tracking_area_identity_t *tracking_area_identity)
{
    c_uint16_t size = sizeof(nas_tracking_area_identity_t);
    nas_tracking_area_identity_t target;

    memcpy(&target, tracking_area_identity, size);
    target.tac = htons(tracking_area_identity->tac);

    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(pkbuf->payload - size, &target, size);

    return size;
}

/* 9.9.3.33 Tracking area identity list
 * M LV 7-97 */
c_int16_t nas_decode_tracking_area_identity_list(nas_tracking_area_identity_list_t *tracking_area_identity_list, pkbuf_t *pkbuf)
{
    c_uint16_t size = 0;
    nas_tracking_area_identity_list_t *source = pkbuf->payload;

    tracking_area_identity_list->length = source->length;
    size = tracking_area_identity_list->length + sizeof(tracking_area_identity_list->length);

    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(tracking_area_identity_list, pkbuf->payload - size, size);

    int i = 0;
    {
        if (tracking_area_identity_list->type == NAS_TRACKING_AREA_IDENTITY_LIST_ONE_PLMN_NON_CONSECUTIVE_TACS)
            for (i = 0; i < tracking_area_identity_list->num + 1 && i < NAS_MAX_TRACKING_AREA_IDENTITY; i++)
                tracking_area_identity_list->type0.tac[i] = ntohs(tracking_area_identity_list->type0.tac[i]);
        else if (tracking_area_identity_list->type == NAS_TRACKING_AREA_IDENTITY_LIST_ONE_PLMN_CONSECUTIVE_TACS)
            tracking_area_identity_list->type1.tac = ntohs(tracking_area_identity_list->type1.tac);
        else if (tracking_area_identity_list->type == NAS_TRACKING_AREA_IDENTITY_LIST_MANY_PLMNS)
            for (i = 0; i < tracking_area_identity_list->num + 1 && i < NAS_MAX_TRACKING_AREA_IDENTITY; i++)
                tracking_area_identity_list->type2.tai[i].tac = ntohs(tracking_area_identity_list->type2.tai[i].tac);
        else
            return -1;
    }

    return size;
}

c_int16_t nas_encode_tracking_area_identity_list(pkbuf_t *pkbuf, nas_tracking_area_identity_list_t *tracking_area_identity_list)
{
    c_uint16_t size = tracking_area_identity_list->length + sizeof(tracking_area_identity_list->length);
    nas_tracking_area_identity_list_t target;

    memcpy(&target, tracking_area_identity_list, sizeof(nas_tracking_area_identity_list_t));
    int i = 0;
    {
        if (tracking_area_identity_list->type == NAS_TRACKING_AREA_IDENTITY_LIST_ONE_PLMN_NON_CONSECUTIVE_TACS)
            for (i = 0; i < tracking_area_identity_list->num + 1 && i < NAS_MAX_TRACKING_AREA_IDENTITY; i++)
                target.type0.tac[i] = htons(tracking_area_identity_list->type0.tac[i]);
        else if (tracking_area_identity_list->type == NAS_TRACKING_AREA_IDENTITY_LIST_ONE_PLMN_CONSECUTIVE_TACS)
            target.type1.tac = htons(tracking_area_identity_list->type1.tac);
        else if (tracking_area_identity_list->type == NAS_TRACKING_AREA_IDENTITY_LIST_MANY_PLMNS)
            for (i = 0; i < tracking_area_identity_list->num + 1 && i < NAS_MAX_TRACKING_AREA_IDENTITY; i++)
                target.type2.tai[i].tac = htons(tracking_area_identity_list->type2.tai[i].tac);
        else
            return -1;
    }

    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(pkbuf->payload - size, &target, size);

    return size;
}

/* 9.9.3.34 UE network capability
 * M LV 3-14 */
c_int16_t nas_decode_ue_network_capability(nas_ue_network_capability_t *ue_network_capability, pkbuf_t *pkbuf)
{
    c_uint16_t size = 0;
    nas_ue_network_capability_t *source = pkbuf->payload;

    ue_network_capability->length = source->length;
    size = ue_network_capability->length + sizeof(ue_network_capability->length);

    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(ue_network_capability, pkbuf->payload - size, size);

    return size;
}

c_int16_t nas_encode_ue_network_capability(pkbuf_t *pkbuf, nas_ue_network_capability_t *ue_network_capability)
{
    c_uint16_t size = ue_network_capability->length + sizeof(ue_network_capability->length);
    nas_ue_network_capability_t target;

    memcpy(&target, ue_network_capability, sizeof(nas_ue_network_capability_t));
    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(pkbuf->payload - size, &target, size);

    return size;
}

/* 9.9.3.36 UE security capability
 * M LV 3-6 */
c_int16_t nas_decode_ue_security_capability(nas_ue_security_capability_t *ue_security_capability, pkbuf_t *pkbuf)
{
    c_uint16_t size = 0;
    nas_ue_security_capability_t *source = pkbuf->payload;

    ue_security_capability->length = source->length;
    size = ue_security_capability->length + sizeof(ue_security_capability->length);

    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(ue_security_capability, pkbuf->payload - size, size);

    return size;
}

c_int16_t nas_encode_ue_security_capability(pkbuf_t *pkbuf, nas_ue_security_capability_t *ue_security_capability)
{
    c_uint16_t size = ue_security_capability->length + sizeof(ue_security_capability->length);
    nas_ue_security_capability_t target;

    memcpy(&target, ue_security_capability, sizeof(nas_ue_security_capability_t));
    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(pkbuf->payload - size, &target, size);

    return size;
}

/* 9.9.3.37 Emergency number list
 * O TLV 5-50 */
c_int16_t nas_decode_emergency_number_list(nas_emergency_number_list_t *emergency_number_list, pkbuf_t *pkbuf)
{
    c_uint16_t size = 0;
    nas_emergency_number_list_t *source = pkbuf->payload;

    emergency_number_list->length = source->length;
    size = emergency_number_list->length + sizeof(emergency_number_list->length);

    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(emergency_number_list, pkbuf->payload - size, size);

    return size;
}

c_int16_t nas_encode_emergency_number_list(pkbuf_t *pkbuf, nas_emergency_number_list_t *emergency_number_list)
{
    c_uint16_t size = emergency_number_list->length + sizeof(emergency_number_list->length);
    nas_emergency_number_list_t target;

    memcpy(&target, emergency_number_list, sizeof(nas_emergency_number_list_t));
    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(pkbuf->payload - size, &target, size);

    return size;
}

/* 9.9.3.4 Authentication response parameter
 * M LV 5-17 */
c_int16_t nas_decode_authentication_response_parameter(nas_authentication_response_parameter_t *authentication_response_parameter, pkbuf_t *pkbuf)
{
    c_uint16_t size = 0;
    nas_authentication_response_parameter_t *source = pkbuf->payload;

    authentication_response_parameter->length = source->length;
    size = authentication_response_parameter->length + sizeof(authentication_response_parameter->length);

    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(authentication_response_parameter, pkbuf->payload - size, size);

    return size;
}

c_int16_t nas_encode_authentication_response_parameter(pkbuf_t *pkbuf, nas_authentication_response_parameter_t *authentication_response_parameter)
{
    c_uint16_t size = authentication_response_parameter->length + sizeof(authentication_response_parameter->length);
    nas_authentication_response_parameter_t target;

    memcpy(&target, authentication_response_parameter, sizeof(nas_authentication_response_parameter_t));
    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(pkbuf->payload - size, &target, size);

    return size;
}

/* 9.9.3.44 Voice domain preference and UE usage setting
 * O TLV 3 */
c_int16_t nas_decode_voice_domain_preference_and_ue_usage_setting(nas_voice_domain_preference_and_ue_usage_setting_t *voice_domain_preference_and_ue_usage_setting, pkbuf_t *pkbuf)
{
    c_uint16_t size = 0;
    nas_voice_domain_preference_and_ue_usage_setting_t *source = pkbuf->payload;

    voice_domain_preference_and_ue_usage_setting->length = source->length;
    size = voice_domain_preference_and_ue_usage_setting->length + sizeof(voice_domain_preference_and_ue_usage_setting->length);

    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(voice_domain_preference_and_ue_usage_setting, pkbuf->payload - size, size);

    return size;
}

c_int16_t nas_encode_voice_domain_preference_and_ue_usage_setting(pkbuf_t *pkbuf, nas_voice_domain_preference_and_ue_usage_setting_t *voice_domain_preference_and_ue_usage_setting)
{
    c_uint16_t size = voice_domain_preference_and_ue_usage_setting->length + sizeof(voice_domain_preference_and_ue_usage_setting->length);
    nas_voice_domain_preference_and_ue_usage_setting_t target;

    memcpy(&target, voice_domain_preference_and_ue_usage_setting, sizeof(nas_voice_domain_preference_and_ue_usage_setting_t));
    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(pkbuf->payload - size, &target, size);

    return size;
}

/* 9.9.3.45 GUTI type
 * O TV 1 */
c_int16_t nas_decode_guti_type(nas_guti_type_t *guti_type, pkbuf_t *pkbuf)
{
    memcpy(guti_type, pkbuf->payload - 1, 1);

    return 0;
}

c_int16_t nas_encode_guti_type(pkbuf_t *pkbuf, nas_guti_type_t *guti_type)
{
    c_uint16_t size = sizeof(nas_guti_type_t);

    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(pkbuf->payload - size, guti_type, size);

    return size;
}

/* 9.9.3.46 Extended DRX parameters
 * O TLV 3 */
c_int16_t nas_decode_extended_drx_parameters(nas_extended_drx_parameters_t *extended_drx_parameters, pkbuf_t *pkbuf)
{
    c_uint16_t size = 0;
    nas_extended_drx_parameters_t *source = pkbuf->payload;

    extended_drx_parameters->length = source->length;
    size = extended_drx_parameters->length + sizeof(extended_drx_parameters->length);

    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(extended_drx_parameters, pkbuf->payload - size, size);

    return size;
}

c_int16_t nas_encode_extended_drx_parameters(pkbuf_t *pkbuf, nas_extended_drx_parameters_t *extended_drx_parameters)
{
    c_uint16_t size = extended_drx_parameters->length + sizeof(extended_drx_parameters->length);
    nas_extended_drx_parameters_t target;

    memcpy(&target, extended_drx_parameters, sizeof(nas_extended_drx_parameters_t));
    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(pkbuf->payload - size, &target, size);

    return size;
}

/* 9.9.3.8 DRX parameter
 * O TV 3 */
c_int16_t nas_decode_drx_parameter(nas_drx_parameter_t *drx_parameter, pkbuf_t *pkbuf)
{
    c_uint16_t size = sizeof(nas_drx_parameter_t);

    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(drx_parameter, pkbuf->payload - size, size);

    return size;
}

c_int16_t nas_encode_drx_parameter(pkbuf_t *pkbuf, nas_drx_parameter_t *drx_parameter)
{
    c_uint16_t size = sizeof(nas_drx_parameter_t);
    nas_drx_parameter_t target;

    memcpy(&target, drx_parameter, size);
    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(pkbuf->payload - size, &target, size);

    return size;
}

/* 9.9.3.9 EMM cause
 * O TV 2 */
c_int16_t nas_decode_emm_cause(nas_emm_cause_t *emm_cause, pkbuf_t *pkbuf)
{
    c_uint16_t size = sizeof(nas_emm_cause_t);

    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(emm_cause, pkbuf->payload - size, size);

    return size;
}

c_int16_t nas_encode_emm_cause(pkbuf_t *pkbuf, nas_emm_cause_t *emm_cause)
{
    c_uint16_t size = sizeof(nas_emm_cause_t);
    nas_emm_cause_t target;

    memcpy(&target, emm_cause, size);
    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(pkbuf->payload - size, &target, size);

    return size;
}

/* 9.9.4.1 Access point name
 * M LV 2-101 */
c_int16_t nas_decode_access_point_name(nas_access_point_name_t *access_point_name, pkbuf_t *pkbuf)
{
    c_uint16_t size = 0;
    nas_access_point_name_t *source = pkbuf->payload;

    access_point_name->length = source->length;
    size = access_point_name->length + sizeof(access_point_name->length);

    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(access_point_name, pkbuf->payload - size, size);

    return size;
}

c_int16_t nas_encode_access_point_name(pkbuf_t *pkbuf, nas_access_point_name_t *access_point_name)
{
    c_uint16_t size = access_point_name->length + sizeof(access_point_name->length);
    nas_access_point_name_t target;

    memcpy(&target, access_point_name, sizeof(nas_access_point_name_t));
    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(pkbuf->payload - size, &target, size);

    return size;
}

/* 9.9.4.11 Protocol configuration options
 * O TLV 3-253 */
c_int16_t nas_decode_protocol_configuration_options(nas_protocol_configuration_options_t *protocol_configuration_options, pkbuf_t *pkbuf)
{
    c_uint16_t size = 0;
    nas_protocol_configuration_options_t *source = pkbuf->payload;

    protocol_configuration_options->length = source->length;
    size = protocol_configuration_options->length + sizeof(protocol_configuration_options->length);

    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(protocol_configuration_options, pkbuf->payload - size, size);

    return size;
}

c_int16_t nas_encode_protocol_configuration_options(pkbuf_t *pkbuf, nas_protocol_configuration_options_t *protocol_configuration_options)
{
    c_uint16_t size = protocol_configuration_options->length + sizeof(protocol_configuration_options->length);
    nas_protocol_configuration_options_t target;

    memcpy(&target, protocol_configuration_options, sizeof(nas_protocol_configuration_options_t));
    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(pkbuf->payload - size, &target, size);

    return size;
}

/* 9.9.4.12 Quality of service
 * O TLV 14-22 */
c_int16_t nas_decode_quality_of_service(nas_quality_of_service_t *quality_of_service, pkbuf_t *pkbuf)
{
    c_uint16_t size = 0;
    nas_quality_of_service_t *source = pkbuf->payload;

    quality_of_service->length = source->length;
    size = quality_of_service->length + sizeof(quality_of_service->length);

    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(quality_of_service, pkbuf->payload - size, size);

    return size;
}

c_int16_t nas_encode_quality_of_service(pkbuf_t *pkbuf, nas_quality_of_service_t *quality_of_service)
{
    c_uint16_t size = quality_of_service->length + sizeof(quality_of_service->length);
    nas_quality_of_service_t target;

    memcpy(&target, quality_of_service, sizeof(nas_quality_of_service_t));
    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(pkbuf->payload - size, &target, size);

    return size;
}

/* 9.9.4.13 Radio priority
 * O TV 1 */
c_int16_t nas_decode_radio_priority(nas_radio_priority_t *radio_priority, pkbuf_t *pkbuf)
{
    memcpy(radio_priority, pkbuf->payload - 1, 1);

    return 0;
}

c_int16_t nas_encode_radio_priority(pkbuf_t *pkbuf, nas_radio_priority_t *radio_priority)
{
    c_uint16_t size = sizeof(nas_radio_priority_t);

    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(pkbuf->payload - size, radio_priority, size);

    return size;
}

/* 9.9.4.13A Re-attempt indicator
 * O TLV 3 */
c_int16_t nas_decode_re_attempt_indicator(nas_re_attempt_indicator_t *re_attempt_indicator, pkbuf_t *pkbuf)
{
    c_uint16_t size = 0;
    nas_re_attempt_indicator_t *source = pkbuf->payload;

    re_attempt_indicator->length = source->length;
    size = re_attempt_indicator->length + sizeof(re_attempt_indicator->length);

    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(re_attempt_indicator, pkbuf->payload - size, size);

    return size;
}

c_int16_t nas_encode_re_attempt_indicator(pkbuf_t *pkbuf, nas_re_attempt_indicator_t *re_attempt_indicator)
{
    c_uint16_t size = re_attempt_indicator->length + sizeof(re_attempt_indicator->length);
    nas_re_attempt_indicator_t target;

    memcpy(&target, re_attempt_indicator, sizeof(nas_re_attempt_indicator_t));
    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(pkbuf->payload - size, &target, size);

    return size;
}

/* 9.9.4.14 Request type
 * M V 1/2 */
c_int16_t nas_decode_request_type(nas_request_type_t *request_type, pkbuf_t *pkbuf)
{
    c_uint16_t size = sizeof(nas_request_type_t);

    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(request_type, pkbuf->payload - size, size);

    return size;
}

c_int16_t nas_encode_request_type(pkbuf_t *pkbuf, nas_request_type_t *request_type)
{
    c_uint16_t size = sizeof(nas_request_type_t);
    nas_request_type_t target;

    memcpy(&target, request_type, size);
    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(pkbuf->payload - size, &target, size);

    return size;
}

/* 9.9.4.17 Transaction identifier
 * O TLV 3-4 */
c_int16_t nas_decode_transaction_identifier(nas_transaction_identifier_t *transaction_identifier, pkbuf_t *pkbuf)
{
    c_uint16_t size = 0;
    nas_transaction_identifier_t *source = pkbuf->payload;

    transaction_identifier->length = source->length;
    size = transaction_identifier->length + sizeof(transaction_identifier->length);

    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(transaction_identifier, pkbuf->payload - size, size);

    return size;
}

c_int16_t nas_encode_transaction_identifier(pkbuf_t *pkbuf, nas_transaction_identifier_t *transaction_identifier)
{
    c_uint16_t size = transaction_identifier->length + sizeof(transaction_identifier->length);
    nas_transaction_identifier_t target;

    memcpy(&target, transaction_identifier, sizeof(nas_transaction_identifier_t));
    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(pkbuf->payload - size, &target, size);

    return size;
}

/* 9.9.4.18 WLAN offload acceptability
 * O TV 1 */
c_int16_t nas_decode_wlan_offload_acceptability(nas_wlan_offload_acceptability_t *wlan_offload_acceptability, pkbuf_t *pkbuf)
{
    memcpy(wlan_offload_acceptability, pkbuf->payload - 1, 1);

    return 0;
}

c_int16_t nas_encode_wlan_offload_acceptability(pkbuf_t *pkbuf, nas_wlan_offload_acceptability_t *wlan_offload_acceptability)
{
    c_uint16_t size = sizeof(nas_wlan_offload_acceptability_t);

    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(pkbuf->payload - size, wlan_offload_acceptability, size);

    return size;
}

/* 9.9.4.19 NBIFOM container
 * O TLV 3-257 */
c_int16_t nas_decode_nbifom_container(nas_nbifom_container_t *nbifom_container, pkbuf_t *pkbuf)
{
    c_uint16_t size = 0;
    nas_nbifom_container_t *source = pkbuf->payload;

    nbifom_container->length = source->length;
    size = nbifom_container->length + sizeof(nbifom_container->length);

    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(nbifom_container, pkbuf->payload - size, size);

    return size;
}

c_int16_t nas_encode_nbifom_container(pkbuf_t *pkbuf, nas_nbifom_container_t *nbifom_container)
{
    c_uint16_t size = nbifom_container->length + sizeof(nbifom_container->length);
    nas_nbifom_container_t target;

    memcpy(&target, nbifom_container, sizeof(nas_nbifom_container_t));
    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(pkbuf->payload - size, &target, size);

    return size;
}

/* 9.9.4.2 APN aggregate maximum bit rate
 * O TLV 4-8 */
c_int16_t nas_decode_apn_aggregate_maximum_bit_rate(nas_apn_aggregate_maximum_bit_rate_t *apn_aggregate_maximum_bit_rate, pkbuf_t *pkbuf)
{
    c_uint16_t size = 0;
    nas_apn_aggregate_maximum_bit_rate_t *source = pkbuf->payload;

    apn_aggregate_maximum_bit_rate->length = source->length;
    size = apn_aggregate_maximum_bit_rate->length + sizeof(apn_aggregate_maximum_bit_rate->length);

    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(apn_aggregate_maximum_bit_rate, pkbuf->payload - size, size);

    return size;
}

c_int16_t nas_encode_apn_aggregate_maximum_bit_rate(pkbuf_t *pkbuf, nas_apn_aggregate_maximum_bit_rate_t *apn_aggregate_maximum_bit_rate)
{
    c_uint16_t size = apn_aggregate_maximum_bit_rate->length + sizeof(apn_aggregate_maximum_bit_rate->length);
    nas_apn_aggregate_maximum_bit_rate_t target;

    memcpy(&target, apn_aggregate_maximum_bit_rate, sizeof(nas_apn_aggregate_maximum_bit_rate_t));
    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(pkbuf->payload - size, &target, size);

    return size;
}

/* 9.9.4.22 Header compression configuration
 * O TLV 5-257 */
c_int16_t nas_decode_header_compression_configuration(nas_header_compression_configuration_t *header_compression_configuration, pkbuf_t *pkbuf)
{
    c_uint16_t size = 0;
    nas_header_compression_configuration_t *source = pkbuf->payload;

    header_compression_configuration->length = source->length;
    size = header_compression_configuration->length + sizeof(header_compression_configuration->length);

    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(header_compression_configuration, pkbuf->payload - size, size);

    header_compression_configuration->max_cid = ntohs(header_compression_configuration->max_cid);

    return size;
}

c_int16_t nas_encode_header_compression_configuration(pkbuf_t *pkbuf, nas_header_compression_configuration_t *header_compression_configuration)
{
    c_uint16_t size = header_compression_configuration->length + sizeof(header_compression_configuration->length);
    nas_header_compression_configuration_t target;

    memcpy(&target, header_compression_configuration, sizeof(nas_header_compression_configuration_t));
    target.max_cid = htons(header_compression_configuration->max_cid);

    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(pkbuf->payload - size, &target, size);

    return size;
}

/* 9.9.4.23 Control plane only indication
 * O TV 1 */
c_int16_t nas_decode_control_plane_only_indication(nas_control_plane_only_indication_t *control_plane_only_indication, pkbuf_t *pkbuf)
{
    memcpy(control_plane_only_indication, pkbuf->payload - 1, 1);

    return 0;
}

c_int16_t nas_encode_control_plane_only_indication(pkbuf_t *pkbuf, nas_control_plane_only_indication_t *control_plane_only_indication)
{
    c_uint16_t size = sizeof(nas_control_plane_only_indication_t);

    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(pkbuf->payload - size, control_plane_only_indication, size);

    return size;
}

/* 9.9.4.26 Extended protocol configuration options
 * O TLV-E 4-65538 */
c_int16_t nas_decode_extended_protocol_configuration_options(nas_extended_protocol_configuration_options_t *extended_protocol_configuration_options, pkbuf_t *pkbuf)
{
    c_uint16_t size = 0;
    nas_extended_protocol_configuration_options_t *source = pkbuf->payload;

    extended_protocol_configuration_options->len = ntohs(source->len);
    size = extended_protocol_configuration_options->len + sizeof(extended_protocol_configuration_options->len);

    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    extended_protocol_configuration_options->data = pkbuf->payload - size + sizeof(extended_protocol_configuration_options->len);

    return size;
}

c_int16_t nas_encode_extended_protocol_configuration_options(pkbuf_t *pkbuf, nas_extended_protocol_configuration_options_t *extended_protocol_configuration_options)
{
    c_uint16_t size = 0;
    c_uint16_t target;

    d_assert(extended_protocol_configuration_options, return -1, "Null param");
    d_assert(extended_protocol_configuration_options->data, return -1, "Null param");

    size = sizeof(extended_protocol_configuration_options->len);
    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    target = htons(extended_protocol_configuration_options->len);
    memcpy(pkbuf->payload - size, &target, size);

    size = extended_protocol_configuration_options->len;
    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(pkbuf->payload - size, extended_protocol_configuration_options->data, size);

    return extended_protocol_configuration_options->len + sizeof(extended_protocol_configuration_options->len);
}

/* 9.9.4.28 Serving PLMN rate control
 * O TLV 4 */
c_int16_t nas_decode_serving_plmn_rate_control(nas_serving_plmn_rate_control_t *serving_plmn_rate_control, pkbuf_t *pkbuf)
{
    c_uint16_t size = 0;
    nas_serving_plmn_rate_control_t *source = pkbuf->payload;

    serving_plmn_rate_control->length = source->length;
    size = serving_plmn_rate_control->length + sizeof(serving_plmn_rate_control->length);

    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(serving_plmn_rate_control, pkbuf->payload - size, size);

    return size;
}

c_int16_t nas_encode_serving_plmn_rate_control(pkbuf_t *pkbuf, nas_serving_plmn_rate_control_t *serving_plmn_rate_control)
{
    c_uint16_t size = serving_plmn_rate_control->length + sizeof(serving_plmn_rate_control->length);
    nas_serving_plmn_rate_control_t target;

    memcpy(&target, serving_plmn_rate_control, sizeof(nas_serving_plmn_rate_control_t));
    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(pkbuf->payload - size, &target, size);

    return size;
}

/* 9.9.4.2A Connectivity type
 * O TV 1 */
c_int16_t nas_decode_connectivity_type(nas_connectivity_type_t *connectivity_type, pkbuf_t *pkbuf)
{
    memcpy(connectivity_type, pkbuf->payload - 1, 1);

    return 0;
}

c_int16_t nas_encode_connectivity_type(pkbuf_t *pkbuf, nas_connectivity_type_t *connectivity_type)
{
    c_uint16_t size = sizeof(nas_connectivity_type_t);

    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(pkbuf->payload - size, connectivity_type, size);

    return size;
}

/* 9.9.4.3 EPS quality of service
 * M LV 2-14 */
c_int16_t nas_decode_eps_quality_of_service(nas_eps_quality_of_service_t *eps_quality_of_service, pkbuf_t *pkbuf)
{
    c_uint16_t size = 0;
    nas_eps_quality_of_service_t *source = pkbuf->payload;

    eps_quality_of_service->length = source->length;
    size = eps_quality_of_service->length + sizeof(eps_quality_of_service->length);

    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(eps_quality_of_service, pkbuf->payload - size, size);

    return size;
}

c_int16_t nas_encode_eps_quality_of_service(pkbuf_t *pkbuf, nas_eps_quality_of_service_t *eps_quality_of_service)
{
    c_uint16_t size = eps_quality_of_service->length + sizeof(eps_quality_of_service->length);
    nas_eps_quality_of_service_t target;

    memcpy(&target, eps_quality_of_service, sizeof(nas_eps_quality_of_service_t));
    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(pkbuf->payload - size, &target, size);

    return size;
}

/* 9.9.4.4 ESM cause
 * O TV 2 */
c_int16_t nas_decode_esm_cause(nas_esm_cause_t *esm_cause, pkbuf_t *pkbuf)
{
    c_uint16_t size = sizeof(nas_esm_cause_t);

    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(esm_cause, pkbuf->payload - size, size);

    return size;
}

c_int16_t nas_encode_esm_cause(pkbuf_t *pkbuf, nas_esm_cause_t *esm_cause)
{
    c_uint16_t size = sizeof(nas_esm_cause_t);
    nas_esm_cause_t target;

    memcpy(&target, esm_cause, size);
    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(pkbuf->payload - size, &target, size);

    return size;
}

/* 9.9.4.5 ESM information transfer flag
 * O TV 1 */
c_int16_t nas_decode_esm_information_transfer_flag(nas_esm_information_transfer_flag_t *esm_information_transfer_flag, pkbuf_t *pkbuf)
{
    memcpy(esm_information_transfer_flag, pkbuf->payload - 1, 1);

    return 0;
}

c_int16_t nas_encode_esm_information_transfer_flag(pkbuf_t *pkbuf, nas_esm_information_transfer_flag_t *esm_information_transfer_flag)
{
    c_uint16_t size = sizeof(nas_esm_information_transfer_flag_t);

    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(pkbuf->payload - size, esm_information_transfer_flag, size);

    return size;
}

/* 9.9.4.6 Linked EPS bearer identity
 * M V 1/2 */
c_int16_t nas_decode_linked_eps_bearer_identity(nas_linked_eps_bearer_identity_t *linked_eps_bearer_identity, pkbuf_t *pkbuf)
{
    c_uint16_t size = sizeof(nas_linked_eps_bearer_identity_t);

    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(linked_eps_bearer_identity, pkbuf->payload - size, size);

    return size;
}

c_int16_t nas_encode_linked_eps_bearer_identity(pkbuf_t *pkbuf, nas_linked_eps_bearer_identity_t *linked_eps_bearer_identity)
{
    c_uint16_t size = sizeof(nas_linked_eps_bearer_identity_t);
    nas_linked_eps_bearer_identity_t target;

    memcpy(&target, linked_eps_bearer_identity, size);
    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(pkbuf->payload - size, &target, size);

    return size;
}

/* 9.9.4.7 LLC service access point identifier
 * O TV 2 */
c_int16_t nas_decode_llc_service_access_point_identifier(nas_llc_service_access_point_identifier_t *llc_service_access_point_identifier, pkbuf_t *pkbuf)
{
    c_uint16_t size = sizeof(nas_llc_service_access_point_identifier_t);

    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(llc_service_access_point_identifier, pkbuf->payload - size, size);

    return size;
}

c_int16_t nas_encode_llc_service_access_point_identifier(pkbuf_t *pkbuf, nas_llc_service_access_point_identifier_t *llc_service_access_point_identifier)
{
    c_uint16_t size = sizeof(nas_llc_service_access_point_identifier_t);
    nas_llc_service_access_point_identifier_t target;

    memcpy(&target, llc_service_access_point_identifier, size);
    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(pkbuf->payload - size, &target, size);

    return size;
}

/* 9.9.4.8 Packet flow Identifier
 * O TLV 3 */
c_int16_t nas_decode_packet_flow_identifier(nas_packet_flow_identifier_t *packet_flow_identifier, pkbuf_t *pkbuf)
{
    c_uint16_t size = 0;
    nas_packet_flow_identifier_t *source = pkbuf->payload;

    packet_flow_identifier->length = source->length;
    size = packet_flow_identifier->length + sizeof(packet_flow_identifier->length);

    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(packet_flow_identifier, pkbuf->payload - size, size);

    return size;
}

c_int16_t nas_encode_packet_flow_identifier(pkbuf_t *pkbuf, nas_packet_flow_identifier_t *packet_flow_identifier)
{
    c_uint16_t size = packet_flow_identifier->length + sizeof(packet_flow_identifier->length);
    nas_packet_flow_identifier_t target;

    memcpy(&target, packet_flow_identifier, sizeof(nas_packet_flow_identifier_t));
    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(pkbuf->payload - size, &target, size);

    return size;
}

/* 9.9.4.9 PDN address
 * M LV 6-14 */
c_int16_t nas_decode_pdn_address(nas_pdn_address_t *pdn_address, pkbuf_t *pkbuf)
{
    c_uint16_t size = 0;
    nas_pdn_address_t *source = pkbuf->payload;

    pdn_address->length = source->length;
    size = pdn_address->length + sizeof(pdn_address->length);

    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(pdn_address, pkbuf->payload - size, size);

    return size;
}

c_int16_t nas_encode_pdn_address(pkbuf_t *pkbuf, nas_pdn_address_t *pdn_address)
{
    c_uint16_t size = pdn_address->length + sizeof(pdn_address->length);
    nas_pdn_address_t target;

    memcpy(&target, pdn_address, sizeof(nas_pdn_address_t));
    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, return -1, "pkbuf_header error");
    memcpy(pkbuf->payload - size, &target, size);

    return size;
}

