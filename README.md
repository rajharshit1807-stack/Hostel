# HostelSync UI Prototype

A simple, dark navy/blue, multi-page-style frontend prototype for the Smart Hostel Management & Complaint Resolution System.

## Run locally on Windows CMD or PowerShell

1. Extract this ZIP file.
2. Open Windows Command Prompt in the extracted `HostelSync_updated` folder.
3. Run:
   ```cmd
   python server.py
   ```
4. Open `http://localhost:8000` in your browser.

## Student registration

- Select **Student** and choose **Create an account** on the login screen.
- Enter your name, unique student ID, choose an available room, and create your own password.
- Passwords must be at least 8 characters; the confirmation field must match. No random password is generated.
- Log in using the student ID and password you created. You can change your password from **My Profile**.

The room list contains 20 sample rooms across Blocks A to E, with available, occupied and partially occupied rooms. All sample residents have Indian names and represent an all-boys hostel. The Student Records page includes the sample room occupants and their weekly breakfast/dinner preferences. Warden Mess Management uses these preferences to calculate daily meal requirements. Newly self-registered students are also added to Student Records; their meal counts appear after they save their selections.

## Important integration note

This version is a frontend prototype. It uses sample room/leave records and browser `localStorage` for demonstration interactions. It does not call the C/C++ programs directly.

The uploaded C/C++ files are separate console programs and do not currently expose an HTTP API. A later integration step is required to connect the browser UI to the existing C/C++ functions without replacing the complaint max-heap, normal FIFO queue, room hash table, leave queue, mess logic, or file handling.

## What is based on the uploaded source

- Complaint categories and priority values (HIGH=3, MEDIUM=2, LOW=1) match `complaint(1).c`.
- Urgent complaints are displayed by priority, with earlier complaint IDs first for equal priority, matching the heap comparison.
- Routine complaints retain ID/insertion order to represent the FIFO queue.
- The seven-day menu matches `mess(1).cpp`.
- Room occupancy fields and student ID/name records follow `room(1).cpp`.
- Leave requests show Student ID and reason only; the uploaded leave structure does not contain date fields.
- The route helper is part of the complaint-resolution flow in the C code, so it is not presented as an invented separate management module.

## Demo data

The complaint seed records match the uploaded `complaints.csv`. Room and leave examples are labelled as demonstration records because no current `rooms.csv` or `leave_requests.csv` data file was supplied. The demo can be reset by clearing this site's local storage in the browser.

## Student self-registration (updated)
- Select **Student** and choose **Create an account** on the login screen.
- Enter your full name, unique student ID and choose a room with available beds.
- Create your own password during registration and confirm it. HostelSync does not generate a random password.
- Log in with the student ID and the password you created. Students can change their password from **My Profile**.
- Accounts and app records are saved in the browser's local storage, so they remain after restarting the local Python server in the same browser.

**Prototype limitation:** This is a local academic UI demonstration. Browser-local accounts are not shared between different browsers/computers and this is not production-grade authentication. The password is stored as a SHA-256 hash in local browser storage, but a real deployed system should use a server-side database, salted password hashing and proper session management.


### Warden room allocation
- In Warden Portal → Room Management, select an unassigned registered student and a room with free beds, then choose **Add student to room**.
- Each room lists its current occupants. Choose **Remove** beside a student to remove that student from the room.
- Removing a student from a room does not delete the student's account or meal preferences. The student remains in Student Records and can be assigned to another room later.
- Room allocation is saved in this browser's local storage in this UI prototype. It is not yet synchronized across different browsers or devices.
