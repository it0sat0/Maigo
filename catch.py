import jwt

decoded_id_token = jwt.decode(id_token,
                              channel_secret,
                              audience=channel_id,
                              issuer='https://access.line.me',
                              algorithms=['HS256'])

# check nonce (Optional. But strongly recommended)
nonce = '_stored_in_session_'
expected_nonce = decoded_id_token.get('nonce')
if nonce != decoded_id_token.get('nonce'):
    raise RuntimeError('invalid nonce')