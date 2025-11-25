# Keys / Credentials

This project uses Google OAuth 2.0 credentials to access the Gmail API for the `mail` builtin.

Where to put the downloaded JSON
- Download the OAuth client secret JSON from Google Cloud Console (OAuth 2.0 Client ID, "Desktop" type recommended).
- Save the file in the `Keys/` directory (this repo's `.gitignore` already excludes `Keys/`).

How to extract `CLIENT_ID` and `CLIENT_SECRET`

1) Quick (using `jq`):

```bash
# adjust the glob if you have a different filename
export CLIENT_ID=$(jq -r '.installed.client_id // .web.client_id' Keys/client_secret_*.json)
export CLIENT_SECRET=$(jq -r '.installed.client_secret // .web.client_secret' Keys/client_secret_*.json)
```

2) Manual: open the JSON file and copy the values for `client_id` and `client_secret` found under the top-level `installed` or `web` object.

Set environment variables

You can export the variables in your shell before running the shell binary:

```bash
export CLIENT_ID="your-client-id.apps.googleusercontent.com"
export CLIENT_SECRET="your-client-secret"
./main
```

Or use the provided `.env.example` as a template and source it.

Security notes
- Do not commit the downloaded JSON or any secrets to version control.
- `Keys/` and `refresh_token.json` are already ignored by git in this repository.
