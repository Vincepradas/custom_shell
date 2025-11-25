# custom_shell

This repository contains a small custom Unix-like shell written in C. It provides a starting point for learning about parsing, execution, and basic shell features.

**Quick summary**
- **Language:** C
- **Structure:** Source files in `src/`, headers in `include/`, built binaries in `build/` (if used by the Makefile)

**Features**
- Basic command parsing and execution
- Simple built-ins and executor logic
- Small Gmail integration to fetch today's emails (via the `mail` builtin)
- `greet` builtin for quick testing

**Project Layout**
- `Makefile`: build rules
- `src/`: implementation files (`executor.c`, `main.c`, `parser.c`, `shell.c`)
- `include/`: public headers (`executor.h`, `parser.h`, `shell.h`)
- `src/utils`: helper utilities (`gmail.c`, `token.c`, `write_callback.c`)
- `build/`: (optional) build outputs

Getting started

1. Build

Run the default build target:

```bash
make
```

This project builds object files into `build/` and links an executable named `main` at the repository root by default. The linker uses `-lcurl -lcjson`, so ensure `libcurl` and `cJSON` development packages are installed on your system.

On Debian/Ubuntu:

```bash
sudo apt update
sudo apt install build-essential libcurl4-openssl-dev libcjson-dev
```

2. Run the shell

After a successful build the runnable binary is `./main` in the project root. Example:

```bash
./main
```

If your `Makefile` is changed to place the binary elsewhere, update the command accordingly (for example, `./build/main`).

Gmail integration (the `mail` builtin)

- **What it does:** the `mail` builtin uses the Gmail API to fetch emails from the last day and prints senders and subjects.
- **Dependencies:** requires a Google OAuth Client ID and Client Secret, and network access. The app uses the `https://www.googleapis.com/auth/gmail.readonly` scope.
- **How to provide credentials:**
	- Create an OAuth 2.0 Client ID (Desktop) in the Google Cloud Console and download the client secret JSON file. You may place the file inside the `Keys/` directory (this repo's `.gitignore` already excludes `Keys/`).
	- Alternatively, set the environment variables `CLIENT_ID` and `CLIENT_SECRET` before running `./main`. For example:

```bash
export CLIENT_ID="your-client-id.apps.googleusercontent.com"
export CLIENT_SECRET="your-client-secret"
./main
```

- **First-time authorization:**
	- When you run the `mail` builtin for the first time, the shell will print an authorization URL. Open it in a browser, sign in, and copy the authorization code.
	- Paste the code into the shell prompt when requested. The program will exchange the code for a refresh token and save it to `refresh_token.json`.
	- `refresh_token.json` is listed in `.gitignore` so the refresh token is not committed.

- **Subsequent runs:** the shell reads `refresh_token.json` to obtain access tokens automatically.

**Google Cloud Console — Setup**

- **Create or select a project:**
	1. Open https://console.cloud.google.com/ and sign in.
	2. Use the project selector (top-left) to choose an existing project or click **NEW PROJECT** to create one.

- **Enable the Gmail API:**
	1. Go to **APIs & Services → Library**.
	2. Search for "Gmail API", open it and click **Enable**.

- **Configure the OAuth consent screen:**
	1. Open **APIs & Services → OAuth consent screen** and select **External** (or the option appropriate for your account).
	2. Fill in **App name**, **User support email**, and **Developer contact email**.
	3. Add the Gmail readonly scope: `https://www.googleapis.com/auth/gmail.readonly`.
	4. If in testing mode, add your account as a **Test user** so you can authorize during development.

- **Create OAuth credentials (Client ID & Secret):**
	1. Go to **APIs & Services → Credentials** and click **Create Credentials → OAuth client ID**.
	2. Choose **Application type: Desktop app** (or appropriate type), give it a name and create.
	3. Download the client secret JSON (filename like `client_secret_...json`) and save it in the `Keys/` directory (this repo ignores `Keys/`).

- **Obtain the authorization code (first-time):**
	1. Export `CLIENT_ID` and `CLIENT_SECRET` (or place the JSON into `Keys/` and extract values). Example:

```bash
export CLIENT_ID="your-client-id.apps.googleusercontent.com"
export CLIENT_SECRET="your-client-secret"
./main
```

	2. Run `mail` inside the shell; it will print an authorization URL. Open it in a browser, sign in and grant permission. Copy the authorization code Google shows and paste it into the shell prompt when requested.
	3. The program exchanges the code and saves a refresh token to `refresh_token.json`.

- **Notes & tips:**
	- Use `access_type=offline` (ensures a refresh token is returned) and `prompt=consent` if you need to force a refresh token on repeat authorizations.
	- If OOB (out-of-band) redirects are restricted for your Google account, create a Desktop client and use a local redirect (e.g. `http://localhost:PORT`) or use the OAuth Playground to retrieve tokens.
	- Quick extraction with `jq` if you saved the JSON to `Keys/`:

```bash
export CLIENT_ID=$(jq -r '.installed.client_id // .web.client_id' Keys/client_secret_*.json)
export CLIENT_SECRET=$(jq -r '.installed.client_secret // .web.client_secret' Keys/client_secret_*.json)
```

Usage examples

- Run a simple command:

```text
$ ./main
> ls -la
<ls output>
> exit
```

- Use the `mail` builtin to fetch today's emails:

```text
> mail
# first run: follow printed URL, paste auth code
Fetching today's emails...
From: Example <example@example.com> | Subject: Hello
```

- Use the `greet` builtin:

```text
> greet Alice
Hello! Alice
```

Notes & testing

- The repository is intentionally minimal to be a learning project. If you change the build rules or binary name, update this README accordingly.
- To validate the build instructions, run `make` and then the produced binary. If something fails, open an issue or share the `Makefile` and build output.
- You may see compiler warnings about unused or undefined static functions in `include/utils/gmail.h` / `src/utils/gmail.c`; these are non-fatal but worth reviewing if you work on that utility.

Security & secrets

- `Keys/` and `refresh_token.json` are ignored by git (see `.gitignore`). Do not commit client secrets or refresh tokens to version control.

Contributing

- Fork the repository and submit pull requests for bug fixes or feature additions.
- Keep functions small and add tests (or example sessions) demonstrating behavior.

Open an issue or reach out in the repo for questions, improvements, or help running the project.
