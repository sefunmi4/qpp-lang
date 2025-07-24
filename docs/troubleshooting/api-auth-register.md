# Troubleshooting `api/auth/register` Failures

If the `api/auth/register` endpoint returns an HTML page or a `404 Not Found` error, the most common cause is an incorrect base URL in the front‑end's Axios configuration. When the base URL points to the wrong server or port, Axios requests may be routed to your development server instead of the API server, resulting in HTML content rather than a JSON response.

## Steps to Verify

1. **Check the Axios Instance** – Look for the file where Axios is configured, commonly named `api.ts`, `axios.ts`, or similar. Ensure that the `baseURL` points to your API server, e.g.:
   ```ts
   import axios from 'axios';

   const api = axios.create({
     baseURL: 'http://localhost:3000', // replace with your API host and port
   });

   export default api;
   ```
2. **Environment Variables** – Many projects set the base URL via environment variables like `VITE_API_URL` or `REACT_APP_API_URL`. Verify these variables in your `.env` files and restart the development server after making changes.
3. **Network Errors** – Confirm that the back‑end server is running and reachable. A misconfigured proxy or an inactive server can also lead to 404 errors.

After updating the base URL, rebuild or restart your front‑end to ensure Axios uses the new configuration.
