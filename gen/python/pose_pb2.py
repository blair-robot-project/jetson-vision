# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: pose.proto

import sys
_b=sys.version_info[0]<3 and (lambda x:x) or (lambda x:x.encode('latin1'))
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from google.protobuf import reflection as _reflection
from google.protobuf import symbol_database as _symbol_database
from google.protobuf import descriptor_pb2
# @@protoc_insertion_point(imports)

_sym_db = _symbol_database.Default()




DESCRIPTOR = _descriptor.FileDescriptor(
  name='pose.proto',
  package='proto',
  serialized_pb=_b('\n\npose.proto\x12\x05proto\"i\n\nCameraPose\x12\t\n\x01x\x18\x01 \x01(\x01\x12\t\n\x01y\x18\x02 \x01(\x01\x12\t\n\x01z\x18\x03 \x01(\x01\x12\x0b\n\x03yaw\x18\x04 \x01(\x01\x12\r\n\x05pitch\x18\x05 \x01(\x01\x12\x0c\n\x04roll\x18\x06 \x01(\x01\x12\x10\n\x08navXTime\x18\x07 \x01(\x05')
)
_sym_db.RegisterFileDescriptor(DESCRIPTOR)




_CAMERAPOSE = _descriptor.Descriptor(
  name='CameraPose',
  full_name='proto.CameraPose',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='x', full_name='proto.CameraPose.x', index=0,
      number=1, type=1, cpp_type=5, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='y', full_name='proto.CameraPose.y', index=1,
      number=2, type=1, cpp_type=5, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='z', full_name='proto.CameraPose.z', index=2,
      number=3, type=1, cpp_type=5, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='yaw', full_name='proto.CameraPose.yaw', index=3,
      number=4, type=1, cpp_type=5, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='pitch', full_name='proto.CameraPose.pitch', index=4,
      number=5, type=1, cpp_type=5, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='roll', full_name='proto.CameraPose.roll', index=5,
      number=6, type=1, cpp_type=5, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='navXTime', full_name='proto.CameraPose.navXTime', index=6,
      number=7, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  options=None,
  is_extendable=False,
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=21,
  serialized_end=126,
)

DESCRIPTOR.message_types_by_name['CameraPose'] = _CAMERAPOSE

CameraPose = _reflection.GeneratedProtocolMessageType('CameraPose', (_message.Message,), dict(
  DESCRIPTOR = _CAMERAPOSE,
  __module__ = 'pose_pb2'
  # @@protoc_insertion_point(class_scope:proto.CameraPose)
  ))
_sym_db.RegisterMessage(CameraPose)


# @@protoc_insertion_point(module_scope)
