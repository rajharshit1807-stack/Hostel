const DAYS = ["Monday","Tuesday","Wednesday","Thursday","Friday","Saturday","Sunday"];
const MENU = [
  ["Aloo Paratha","Rajma Rice","Samosa","Roti, Dal and Paneer"],
  ["Poha and Tea","Chole Chawal","Bread Pakora","Mix Veg, Roti and Dal"],
  ["Idli Sambhar","Kadhi Chawal","Veg Sandwich","Aloo Gobi, Roti and Rice"],
  ["Puri Sabzi","Dal Tadka, Roti and Rice","Biscuits and Tea","Paneer, Roti and Dal"],
  ["Upma and Banana","Veg Pulao and Raita","Pav Bhaji","Dal Makhani, Roti and Rice"],
  ["Chole Bhature","Paneer Butter Masala and Roti","Maggi","Veg Biryani and Raita"],
  ["Dosa and Chutney","Special Thali","Pasta","Matar Paneer, Roti and Kheer"]
];
const COMPLAINT_TYPES = [
 ["Main door handle broken",2],["Geyser not working",2],["Short circuit in switch board",3],
 ["Toilet flush damaged",2],["Room cleaning",1],["Fan in the hall not working",2],
 ["Electricity spark in the geyser",3],["Wi-Fi not working",2],["Tubelight fused",2],
 ["Termite infestation in the almirah",2],["Roommate and study problem",1],
 ["Security and leave management problem",2],["Laundry management",1],
 ["Lack of emergency support",3],["Difficulty reporting harassment or ragging",3]
];
const seed = {
  complaints: [
    {id:1,studentId:107,room:401,description:"Short circuit in switch board",priority:3,status:"Pending"},
    {id:2,studentId:103,room:102,description:"Electricity spark in the geyser",priority:3,status:"Pending"},
    {id:3,studentId:104,room:201,description:"Wi-Fi not working",priority:2,status:"Pending"},
    {id:4,studentId:101,room:101,description:"Main door handle broken",priority:2,status:"Pending"},
    {id:5,studentId:105,room:301,description:"Room cleaning",priority:1,status:"Pending"},
    {id:6,studentId:102,room:101,description:"Toilet flush damaged",priority:2,status:"Resolved"},
    {id:7,studentId:106,room:301,description:"Tubelight fused",priority:2,status:"Resolved"}
  ],
  rooms: [
    {room:"101",block:"A",capacity:3,students:[{id:101,name:"Rahul Mehta"},{id:102,name:"Aman Sharma"}]},
    {room:"102",block:"A",capacity:3,students:[{id:103,name:"Vikas Rawat"}]},
    {room:"103",block:"A",capacity:2,students:[]},
    {room:"104",block:"A",capacity:3,students:[{id:108,name:"Arjun Singh"},{id:109,name:"Rohit Joshi"},{id:110,name:"Kunal Negi"}]},
    {room:"105",block:"A",capacity:2,students:[{id:111,name:"Deepak Kumar"}]},
    {room:"201",block:"B",capacity:3,students:[{id:104,name:"Mohit Rana"},{id:201,name:"Sahil Verma"}]},
    {room:"202",block:"B",capacity:3,students:[{id:202,name:"Nikhil Bisht"},{id:203,name:"Ankit Rawat"}]},
    {room:"203",block:"B",capacity:2,students:[]},
    {room:"204",block:"B",capacity:4,students:[{id:204,name:"Harsh Gupta"},{id:205,name:"Aarav Joshi"},{id:206,name:"Manish Pal"}]},
    {room:"205",block:"B",capacity:3,students:[{id:207,name:"Yash Thakur"}]},
    {room:"301",block:"C",capacity:2,students:[{id:105,name:"Mohit Rawat"},{id:106,name:"Sahil Gupta"}]},
    {room:"302",block:"C",capacity:3,students:[{id:301,name:"Rishabh Negi"}]},
    {room:"303",block:"C",capacity:3,students:[{id:302,name:"Abhishek Tiwari"},{id:303,name:"Sumit Verma"},{id:304,name:"Pranav Shah"}]},
    {room:"304",block:"C",capacity:2,students:[]},
    {room:"305",block:"C",capacity:4,students:[{id:305,name:"Shivam Joshi"},{id:306,name:"Dev Patel"}]},
    {room:"401",block:"D",capacity:4,students:[{id:107,name:"Karan Pandey"},{id:401,name:"Abhishek Thapa"},{id:402,name:"Sumit Vashisht"}]},
    {room:"402",block:"D",capacity:3,students:[{id:403,name:"Tarun Singh"},{id:404,name:"Gaurav Negi"},{id:405,name:"Amit Kumar"}]},
    {room:"403",block:"D",capacity:2,students:[{id:406,name:"Naveen Rawat"}]},
    {room:"501",block:"E",capacity:3,students:[]},
    {room:"502",block:"E",capacity:4,students:[{id:501,name:"Piyush Sharma"},{id:502,name:"Vivek Joshi"}]}
  ],
  leaves: [
    {id:1,studentId:104,reason:"Family function",status:"Pending"},
    {id:2,studentId:201,reason:"Medical appointment",status:"Pending"}
  ],
  students: [],
  accounts: [],
  meals: {}
};
let db = loadDB();
let currentRole = "student";
let currentUser = null;
let currentPage = "dashboard";
let toastTimer;

const studentNav = [
  ["dashboard","▦","Dashboard"],["mess","♨","Food & Mess"],["complaints","⚑","Complaints"],
  ["leave","▣","Hostel Leave"],["rooms","▤","Room Availability"],["profile","◉","My Profile"]
];
const wardenNav = [
  ["dashboard","▦","Dashboard"],["complaints","⚑","Complaints"],["rooms","▤","Room Management"],
  ["leave","▣","Leave Requests"],["mess","♨","Mess Management"],["students","♙","Student Records"]
];
function loadDB() {
  try {
    const saved=localStorage.getItem("hostelsync-demo-v2");
    const data=saved?JSON.parse(saved):JSON.parse(JSON.stringify(seed));
    data.accounts=data.accounts||[]; data.students=data.students||[]; data.meals=data.meals||{};
    // Add the Indian-named sample occupants from the room dataset to Student Records.
    // Keep any students who registered themselves in the browser.
    const knownIds=new Set(data.students.map(st=>String(st.id)));
    (data.rooms||[]).forEach(room=>(room.students||[]).forEach(st=>{
      if(!knownIds.has(String(st.id))){data.students.push({id:st.id,name:st.name});knownIds.add(String(st.id));}
    }));
    // Give the sample students varied weekly breakfast/dinner preferences so
    // the warden's food-requirement totals are populated on first load.
    data.students.forEach((st,index)=>{
      const key=String(st.id);
      if(!Object.prototype.hasOwnProperty.call(data.meals,key)){
        const picks=[];
        for(let day=0;day<7;day++){
          if((index+day)%3!==0) picks.push(`${day}-B`);
          if((index*2+day)%4!==0) picks.push(`${day}-D`);
        }
        data.meals[key]=picks;
      }
    });
    return data;
  } catch(e) { return JSON.parse(JSON.stringify(seed)); }
}
function saveDB() { localStorage.setItem("hostelsync-demo-v2",JSON.stringify(db)); }
function esc(s) { return String(s??"").replace(/[&<>"']/g,c=>({"&":"&amp;","<":"&lt;",">":"&gt;",'"':"&quot;","'":"&#39;"}[c])); }
function priorityName(p) { return p===3?"High":p===2?"Medium":"Low"; }
function priorityClass(p) { return p===3?"high":p===2?"medium":"low"; }
function pill(text,cls="neutral") { return `<span class="pill ${cls}">${esc(text)}</span>`; }
function statusPill(s) { return pill(s,s.toLowerCase()); }
function toast(msg) { const el=document.getElementById("toast");el.textContent=msg;el.classList.add("show");clearTimeout(toastTimer);toastTimer=setTimeout(()=>el.classList.remove("show"),2600); }
function roomForStudent(id) { return db.rooms.find(r=>r.students.some(s=>String(s.id)===String(id))); }
function studentName(id) { return db.students.find(s=>String(s.id)===String(id))?.name || "Student"; }
function pendingComplaints() { return db.complaints.filter(c=>c.status!=="Resolved"); }
function orderedUrgent() { return pendingComplaints().filter(c=>c.priority===3).sort((a,b)=>b.priority-a.priority||a.id-b.id); }
function orderedNormal() { return pendingComplaints().filter(c=>c.priority!==3).sort((a,b)=>a.id-b.id); }
function go(page) { currentPage=page; renderNav(); renderPage(); document.getElementById("topbarTitle").textContent=(currentRole==="student"?studentNav:wardenNav).find(n=>n[0]===page)?.[2]||"Dashboard"; }
function renderNav() {
  const nav=currentRole==="student"?studentNav:wardenNav;
  document.getElementById("sidebarRole").textContent=currentRole==="student"?"Student Portal":"Warden Portal";
  document.getElementById("sidebarNav").innerHTML=nav.map(([id,icon,label])=>`<button class="nav-item ${currentPage===id?"active":""}" data-page="${id}"><span class="nav-icon">${icon}</span>${label}</button>`).join("");
  document.querySelectorAll(".nav-item").forEach(b=>b.addEventListener("click",()=>go(b.dataset.page)));
}
function loginRole(role) {
  currentRole=role;
  document.querySelectorAll(".role-btn").forEach(b=>b.classList.toggle("active",b.dataset.role===role));
  document.getElementById("loginIdLabel").textContent=role==="student"?"Student ID":"Warden username";
  document.getElementById("loginId").placeholder=role==="student"?"Enter your student ID":"Enter username";
  document.getElementById("passwordWrap").classList.remove("hidden");
  document.getElementById("registerToggle").classList.toggle("hidden",role!=="student");
  document.getElementById("registerForm").classList.add("hidden");
  document.getElementById("credentialResult").classList.add("hidden");
  document.getElementById("loginError").textContent="";
}
function refreshRoomChoices() {
  const select=document.getElementById("regRoom");
  const available=db.rooms.filter(r=>r.students.length<r.capacity);
  select.innerHTML=available.map(r=>`<option value="${esc(r.room)}">Room ${esc(r.room)} - Block ${esc(r.block)} (${r.capacity-r.students.length} bed(s) available)</option>`).join("");
  if(!available.length) select.innerHTML='<option value="">No rooms currently available</option>';
}
async function hashPassword(value) {
  const bytes=new TextEncoder().encode(value);
  const digest=await crypto.subtle.digest("SHA-256",bytes);
  return Array.from(new Uint8Array(digest),b=>b.toString(16).padStart(2,"0")).join("");
}
function enterPortal(user,role) {
  currentUser=user; currentRole=role;
  document.getElementById("loginScreen").classList.add("hidden");
  document.getElementById("app").classList.remove("hidden");
  document.getElementById("userName").textContent=currentUser.name;
  document.getElementById("userMeta").textContent=role==="student"?`Student ID: ${currentUser.id}`:"Warden account";
  document.getElementById("userAvatar").textContent=role==="student"?currentUser.name.charAt(0).toUpperCase():"W";
  go("dashboard");
}
document.querySelectorAll(".role-btn").forEach(b=>b.addEventListener("click",()=>loginRole(b.dataset.role)));
document.getElementById("loginForm").addEventListener("submit",async e=>{
  e.preventDefault();
  const id=document.getElementById("loginId").value.trim();
  const pass=document.getElementById("loginPassword").value;
  if(currentRole==="warden") {
    if(id!=="admin"||pass!=="admin123"){document.getElementById("loginError").textContent="Demo warden login is incorrect.";return;}
    enterPortal({id:"admin",name:"Hostel Warden"},"warden");
  } else {
    const account=db.accounts.find(a=>String(a.id).toLowerCase()===id.toLowerCase());
    if(!account||account.passwordHash!==await hashPassword(pass)){document.getElementById("loginError").textContent="Student ID or password is incorrect. Please register if you are a new student.";return;}
    enterPortal({id:account.id,name:account.name},"student");
  }
});
document.getElementById("showRegister").addEventListener("click",()=>{
  document.getElementById("registerForm").classList.remove("hidden");
  document.getElementById("loginForm").classList.add("hidden");
  document.getElementById("registerToggle").classList.add("hidden");
  document.getElementById("credentialResult").classList.add("hidden");
  refreshRoomChoices();
});
document.getElementById("hideRegister").addEventListener("click",()=>{
  document.getElementById("registerForm").classList.add("hidden");
  document.getElementById("loginForm").classList.remove("hidden");
  document.getElementById("registerToggle").classList.remove("hidden");
  document.getElementById("registerForm").reset();
});
document.getElementById("registerForm").addEventListener("submit",async e=>{
  e.preventDefault();
  const name=document.getElementById("regName").value.trim();
  const id=document.getElementById("regId").value.trim();
  const roomNo=document.getElementById("regRoom").value;
  const password=document.getElementById("regPassword").value;
  const confirmPassword=document.getElementById("regConfirmPassword").value;
  const error=document.getElementById("registerError"); error.textContent="";
  if(!name||!id||!roomNo||!password||!confirmPassword){error.textContent="Please fill all fields.";return;}
  if(password.length<8){error.textContent="Password must contain at least 8 characters.";return;}
  if(password!==confirmPassword){error.textContent="Passwords do not match.";return;}
  if(db.accounts.some(a=>String(a.id).toLowerCase()===id.toLowerCase())||db.students.some(s=>String(s.id).toLowerCase()===id.toLowerCase())){
    error.textContent="This student ID is already registered. Please use your own unique ID.";return;
  }
  const room=db.rooms.find(r=>String(r.room)===roomNo);
  if(!room||room.students.length>=room.capacity){error.textContent="This room is full. Please select another room.";refreshRoomChoices();return;}
  const account={id,name,passwordHash:await hashPassword(password),room:roomNo};
  db.accounts.push(account); db.students.push({id,name});
  room.students.push({id,name}); saveDB();
  document.getElementById("registerForm").classList.add("hidden");
  document.getElementById("loginForm").classList.remove("hidden");
  document.getElementById("registerToggle").classList.remove("hidden");
  document.getElementById("loginId").value=id;
  document.getElementById("loginPassword").value="";
  document.getElementById("registerForm").reset();
  const result=document.getElementById("credentialResult");
  result.innerHTML=`<strong>Registration successful!</strong><span>Student ID: <b>${esc(id)}</b></span><p class="inline-help">Your account has been created. Log in using the password you chose during registration.</p>`;
  result.classList.remove("hidden");
});
document.getElementById("logoutBtn").addEventListener("click",()=>{
  currentUser=null;document.getElementById("app").classList.add("hidden");document.getElementById("loginScreen").classList.remove("hidden");
  document.getElementById("loginForm").reset();loginRole("student");
});
function pageHead(title,desc,action="") { return `<div class="page-heading"><div><h1>${title}</h1><p>${desc}</p></div>${action}</div>`; }
function stat(value,label,note="") { return `<div class="stat-card"><div class="stat-value">${value}</div><div class="stat-label">${label}</div>${note?`<div class="stat-note">${note}</div>`:""}</div>`; }
function quick(title,desc,page) { return `<button class="quick-link" data-quick="${page}"><strong>${title}</strong><span>${desc}</span></button>`; }
function renderPage() {
  const root=document.getElementById("pageContent");
  const pages={
    dashboard:currentRole==="student"?studentDashboard:wardenDashboard,
    complaints:currentRole==="student"?studentComplaints:wardenComplaints,
    rooms:currentRole==="student"?studentRooms:wardenRooms,
    leave:currentRole==="student"?studentLeave:wardenLeave,
    mess:currentRole==="student"?studentMess:wardenMess,
    profile:studentProfile,students:wardenStudents
  };
  root.innerHTML=(pages[currentPage]||studentDashboard)();
  root.querySelectorAll("[data-quick]").forEach(b=>b.addEventListener("click",()=>go(b.dataset.quick)));
  bindPageEvents(root);
}
function studentDashboard() {
  const myC=db.complaints.filter(c=>String(c.studentId)===String(currentUser.id));
  const myL=db.leaves.filter(l=>String(l.studentId)===String(currentUser.id));
  const room=roomForStudent(currentUser.id);
  return pageHead("Student Dashboard","Your hostel information and requests at one place.")+
  `<div class="stats-grid">${stat(room?room.room:"—","Room number",room?`Block ${room.block}`:"No room record")}${stat(myC.filter(c=>c.status!=="Resolved").length,"Open complaints")}${stat(myL.filter(l=>l.status==="Pending").length,"Pending leave requests")}${stat(db.meals[currentUser.id]?.length||0,"Meal selections","For this demo week")}</div>
  <div class="content-grid"><section class="panel"><div class="panel-title"><h2>Quick access</h2></div><div class="quick-grid">${quick("Food & Mess","View menu and select meals","mess")}${quick("Register Complaint","Report a hostel issue","complaints")}${quick("Hostel Leave","Submit a leave request","leave")}${quick("Room Availability","Check room occupancy","rooms")}</div></section>
  <section class="panel"><div class="panel-title"><h2>My recent requests</h2></div>${myC.length||myL.length?`<div class="table-wrap"><table><thead><tr><th>Request</th><th>Status</th></tr></thead><tbody>${myC.slice(-3).map(c=>`<tr><td>Complaint #${c.id} · ${esc(c.description)}</td><td>${statusPill(c.status)}</td></tr>`).join("")}${myL.slice(-3).map(l=>`<tr><td>Leave request #${l.id}</td><td>${statusPill(l.status)}</td></tr>`).join("")}</tbody></table></div>`:`<div class="empty-state">No requests submitted yet.</div>`}</section></div>`;
}
function wardenDashboard() {
  const pending=pendingComplaints(),rooms=db.rooms,leaves=db.leaves.filter(l=>l.status==="Pending");
  const urgent=orderedUrgent();
  const free=rooms.reduce((n,r)=>n+r.capacity-r.students.length,0);
  return pageHead("Warden Dashboard","Overview of complaints, leave requests and room occupancy.")+
  `<div class="stats-grid">${stat(pending.length,"Pending complaints",`${urgent.length} urgent`)}${stat(urgent.length,"Urgent complaints","Priority queue")}${stat(leaves.length,"Pending leave requests","Awaiting review")}${stat(free,"Available beds","Based on room records")}</div>
  <div class="content-grid"><section class="panel"><div class="panel-title"><div><h2>Next complaints to resolve</h2><p>Urgent complaints first, then routine FIFO order.</p></div><button class="small-btn blue" data-quick="complaints">View all</button></div>${[...urgent,...orderedNormal()].slice(0,4).map(c=>`<div class="request-row"><div><strong>#${c.id} · Room ${c.room}</strong><div class="muted">${esc(c.description)}</div><small class="muted">Student ${c.studentId}</small></div>${pill(priorityName(c.priority),priorityClass(c.priority))}</div>`).join("")||`<div class="empty-state">No pending complaints.</div>`}</section>
  <section class="panel"><div class="panel-title"><div><h2>Leave requests</h2><p>Requests waiting for a decision.</p></div><button class="small-btn blue" data-quick="leave">Review</button></div>${leaves.slice(0,4).map(l=>`<div class="request-row"><div><strong>${esc(studentName(l.studentId))} · ${l.studentId}</strong><div class="muted">${esc(l.reason)}</div></div>${pill("Pending","pending")}</div>`).join("")||`<div class="empty-state">No pending leave requests.</div>`}</section></div>
  <section class="panel"><div class="panel-title"><h2>Room occupancy snapshot</h2><button class="small-btn" data-quick="rooms">Open rooms</button></div><div class="room-grid">${rooms.slice(0,6).map(roomCard).join("")}</div></section>`;
}
function studentComplaints() {
  const mine=db.complaints.filter(c=>String(c.studentId)===String(currentUser.id));
  return pageHead("My Complaints","Register a complaint and follow its status.")+
  `<div class="content-grid"><section class="panel form-card"><div class="panel-title"><h2>Register a complaint</h2></div>
  <form id="complaintForm"><label for="complaintType">Complaint type</label><select id="complaintType" required><option value="">Select a problem</option>${COMPLAINT_TYPES.map((x,i)=>`<option value="${i}">${esc(x[0])} · ${priorityName(x[1])}</option>`).join("")}</select>
  <div class="form-grid"><div><label for="complaintRoom">Room number</label><input id="complaintRoom" type="number" min="1" value="${roomForStudent(currentUser.id)?.room||""}" required></div><div><label>Student ID</label><input value="${currentUser.id}" disabled></div></div>
  <p class="inline-help">Priority is assigned using the complaint category rules in your C module.</p><div class="form-actions"><button class="primary-btn" type="submit">Submit complaint</button></div></form></section>
  <section class="panel"><div class="panel-title"><h2>My complaint history</h2><span class="pill neutral">${mine.length} records</span></div>${complaintTable(mine,false)}</section></div>`;
}
function wardenComplaints() {
  const urgent=orderedUrgent(),normal=orderedNormal(),resolved=db.complaints.filter(c=>c.status==="Resolved");
  return pageHead("Complaint Management","Review urgent complaints first, followed by routine complaints.")+
  `<div class="notice">The demo follows your code's rule: HIGH complaints are handled by priority (max-heap; older ID wins ties). MEDIUM and LOW complaints remain in FIFO order.</div>
  <section class="panel"><div class="panel-title"><div><h2>Urgent complaint queue</h2><p>Priority queue · ${urgent.length} pending</p></div>${pill("HIGH PRIORITY","high")}</div>${complaintTable(urgent,true)}</section>
  <section class="panel"><div class="panel-title"><div><h2>Routine complaint queue</h2><p>FIFO queue · ${normal.length} pending</p></div>${pill("FIFO","neutral")}</div>${complaintTable(normal,true)}</section>
  <section class="panel"><div class="panel-title"><h2>Resolved history</h2><span class="pill resolved">${resolved.length} resolved</span></div>${complaintTable(resolved,false)}</section>`;
}
function complaintTable(items,canResolve) {
  if(!items.length)return `<div class="empty-state">No complaint records in this section.</div>`;
  return `<div class="table-wrap"><table><thead><tr><th>ID</th><th>Student</th><th>Room</th><th>Problem</th><th>Priority</th><th>Status</th>${canResolve?"<th>Action</th>":""}</tr></thead><tbody>${items.map(c=>`<tr><td>#${c.id}</td><td>${c.studentId}</td><td>${c.room}</td><td>${esc(c.description)}</td><td>${pill(priorityName(c.priority),priorityClass(c.priority))}</td><td>${statusPill(c.status)}</td>${canResolve?`<td><button class="small-btn green" data-resolve="${c.id}">Mark resolved</button></td>`:""}</tr>`).join("")}</tbody></table></div>`;
}
function roomCard(r) {
  const occupied=r.students.length,free=r.capacity-occupied,status=free===0?"Occupied":occupied===0?"Available":"Partially Occupied";
  return `<div class="room-card ${status==="Occupied"?"full-room":""}"><div class="room-top"><strong>Room ${esc(r.room)}</strong>${pill(status,status==="Available"?"available":status==="Occupied"?"full":"partial")}</div><div class="room-meta">Block: <b>${esc(r.block)}</b><br>Occupancy: <b>${occupied} / ${r.capacity} beds</b><br>Available beds: <b>${free}</b></div>${currentRole==="warden"?`<div class="action-row" style="margin-top:12px"><button class="small-btn" data-room-students="${r.room}">View occupants</button></div>`:`<div class="inline-help">${occupied===0?"No occupants listed":`${occupied} student(s) in this room`}</div>`}</div>`;
}
function studentRooms() {
  const room=roomForStudent(currentUser.id);
  return pageHead("Room Availability","View room occupancy and available beds in the all-boys hostel.")+
  `<div class="notice">Room records in this screen are sample records for the UI demonstration. Replace them with the current rooms.csv data during backend integration.</div>
  ${room?`<section class="panel"><div class="panel-title"><h2>Your room</h2></div>${roomCard(room)}</section>`:""}
  <section class="panel"><div class="panel-title"><h2>Hostel room status</h2></div><div class="room-grid">${db.rooms.map(roomCard).join("")}</div></section>`;
}
function wardenRoomCard(r) {
  const occupied=r.students.length,free=r.capacity-occupied;
  const status=free===0?"Occupied":occupied===0?"Available":"Partially Occupied";
  const occupants=occupied?r.students.map(st=>`<div class="occupant-row"><span>${esc(st.name)} <small>(${esc(st.id)})</small></span><button class="small-btn red" data-remove-room-student="${esc(r.room)}" data-student-id="${esc(st.id)}">Remove</button></div>`).join(""):`<p class="inline-help">No students assigned</p>`;
  return `<div class="room-card ${status==="Occupied"?"full-room":""}"><div class="room-top"><strong>Room ${esc(r.room)}</strong>${pill(status,status==="Available"?"available":status==="Occupied"?"full":"partial")}</div><div class="room-meta">Block: <b>${esc(r.block)}</b><br>Occupancy: <b>${occupied} / ${r.capacity} beds</b><br>Available beds: <b>${free}</b></div><div class="occupant-list"><b>Students in this room</b>${occupants}</div></div>`;
}
function wardenRooms() {
  const assigned=new Set(db.rooms.flatMap(r=>r.students.map(st=>String(st.id))));
  const unassigned=db.students.filter(st=>!assigned.has(String(st.id)));
  const roomOptions=db.rooms.filter(r=>r.students.length<r.capacity);
  return pageHead("Room Management","Assign students to rooms or remove them from their current room.")+ 
  `<div class="notice">Use this section to manage room allocation in the all-boys hostel. Removing a student from a room does not delete the student's account or meal preferences.</div>
  <section class="panel form-card"><div class="panel-title"><h2>Add student to a room</h2><span class="pill neutral">${unassigned.length} unassigned</span></div>
  ${unassigned.length&&roomOptions.length?`<form id="assignRoomForm"><div class="form-grid"><div><label for="assignStudent">Student</label><select id="assignStudent" required><option value="">Select student</option>${unassigned.map(st=>`<option value="${esc(st.id)}">${esc(st.name)} (${esc(st.id)})</option>`).join("")}</select></div><div><label for="assignRoom">Room</label><select id="assignRoom" required><option value="">Select room</option>${roomOptions.map(r=>`<option value="${esc(r.room)}">Room ${esc(r.room)} · Block ${esc(r.block)} · ${r.capacity-r.students.length} bed(s) free</option>`).join("")}</select></div></div><div class="form-actions"><button class="primary-btn" type="submit">Add student to room</button></div></form>`:`<div class="empty-state">${!unassigned.length?"All registered students are already assigned to a room.":"No rooms currently have available beds."}</div>`}</section>
  <section class="panel"><div class="panel-title"><h2>Room occupancy</h2><span class="pill neutral">${db.rooms.length} rooms</span></div><div class="room-grid">${db.rooms.map(wardenRoomCard).join("")}</div></section>`;
}
function studentLeave() {
  const mine=db.leaves.filter(l=>String(l.studentId)===String(currentUser.id));
  return pageHead("Hostel Leave","Submit a leave request and check its current status.")+
  `<div class="notice warn">Your current C++ leave structure stores Student ID and Reason only. Date range fields are not part of the uploaded implementation, so this form does not add them.</div>
  <div class="content-grid"><section class="panel form-card"><div class="panel-title"><h2>Submit leave request</h2></div><form id="leaveForm"><label>Student ID</label><input value="${currentUser.id}" disabled><label for="leaveReason">Reason for leave</label><textarea id="leaveReason" maxlength="180" placeholder="Enter your reason..." required></textarea><div class="form-actions"><button class="primary-btn" type="submit">Submit request</button></div></form></section>
  <section class="panel"><div class="panel-title"><h2>My leave requests</h2><span class="pill neutral">${mine.length} records</span></div>${leaveTable(mine,false)}</section></div>`;
}
function wardenLeave() {
  return pageHead("Leave Requests","Review requests in the order they were submitted.")+
  `<div class="notice">The uploaded LeaveQueue uses FIFO processing and stores Student ID and reason. Approval/rejection is supported; dates are not currently stored by this module.</div>
  <section class="panel"><div class="panel-title"><h2>All leave requests</h2><span class="pill pending">${db.leaves.filter(l=>l.status==="Pending").length} pending</span></div>${leaveTable(db.leaves,true)}</section>`;
}
function leaveTable(items,actions) {
  if(!items.length)return `<div class="empty-state">No leave requests found.</div>`;
  return `<div class="table-wrap"><table><thead><tr><th>Request ID</th><th>Student</th><th>Reason</th><th>Status</th>${actions?"<th>Action</th>":""}</tr></thead><tbody>${items.map(l=>`<tr><td>#${l.id}</td><td>${esc(studentName(l.studentId))} (${l.studentId})</td><td>${esc(l.reason)}</td><td>${statusPill(l.status)}</td>${actions&&l.status==="Pending"?`<td><div class="action-row"><button class="small-btn green" data-leave="${l.id}" data-decision="Approved">Approve</button><button class="small-btn red" data-leave="${l.id}" data-decision="Rejected">Reject</button></div></td>`:actions?"<td>—</td>":""}</tr>`).join("")}</tbody></table></div>`;
}
function menuCards() {
  return `<div class="menu-grid">${DAYS.map((day,i)=>`<div class="menu-day"><h3>${day}</h3><div class="meal-line"><b>Breakfast</b><span>${esc(MENU[i][0])}</span></div><div class="meal-line"><b>Lunch</b><span>${esc(MENU[i][1])}</span></div><div class="meal-line"><b>Snacks</b><span>${esc(MENU[i][2])}</span></div><div class="meal-line"><b>Dinner</b><span>${esc(MENU[i][3])}</span></div></div>`).join("")}</div>`;
}
function studentMess() {
  const selected=db.meals[currentUser.id]||[];
  return pageHead("Food & Mess","View the weekly menu and select breakfast or dinner.")+
  `<div class="notice">The menu below is taken from your uploaded mess.cpp. Meal selections are temporary demo data in this browser; the current C++ mess module does not save them to a file.</div>
  <section class="panel"><div class="panel-title"><h2>Weekly mess menu</h2></div>${menuCards()}</section>
  <section class="panel"><div class="panel-title"><h2>My meal selection</h2></div><form id="mealForm"><div class="form-grid"><div><label for="mealDay">Select day</label><select id="mealDay">${DAYS.map((d,i)=>`<option value="${i}">${d}</option>`).join("")}</select></div><div class="field-full"><label>Meals required</label><label class="check-row"><input type="checkbox" id="breakfastCheck"> Breakfast</label><label class="check-row"><input type="checkbox" id="dinnerCheck"> Dinner</label></div></div><div class="form-actions"><button class="primary-btn" type="submit">Save selection</button></div></form></section>`;
}
function wardenMess() {
  const counts=DAYS.map((d,i)=>{let b=0,dn=0;Object.values(db.meals).forEach(arr=>{if(arr.includes(`${i}-B`))b++;if(arr.includes(`${i}-D`))dn++;});return {day:d,b,dn};});
  return pageHead("Mess Management","View the weekly menu and estimate meal requirements.")+
  `<div class="notice">Menu entries match the uploaded C++ source. The current code calculates breakfast and dinner demand from in-memory student selections; this demo shows selections made in this browser only.</div>
  <section class="panel"><div class="panel-title"><h2>Weekly menu</h2></div>${menuCards()}</section>
  <section class="panel"><div class="panel-title"><h2>Food requirement</h2><span class="pill neutral">Demo selections</span></div><div class="table-wrap"><table><thead><tr><th>Day</th><th>Breakfast required</th><th>Dinner required</th></tr></thead><tbody>${counts.map(c=>`<tr><td>${c.day}</td><td>${c.b} student(s)</td><td>${c.dn} student(s)</td></tr>`).join("")}</tbody></table></div></section>`;
}
function studentProfile() {
  const room=roomForStudent(currentUser.id);
  return pageHead("My Profile","View your hostel details and change your login password.")+
  `<section class="panel"><div class="panel-title"><h2>Student details</h2></div><div class="profile-list"><div>Student name</div><div>${esc(currentUser.name)}</div><div>Student ID</div><div>${esc(currentUser.id)}</div><div>Room number</div><div>${room?esc(room.room):"Not available"}</div><div>Block</div><div>${room?esc(room.block):"Not available"}</div></div></section>
  <section class="panel form-card"><div class="panel-title"><h2>Change password</h2></div><form id="passwordChangeForm"><label for="oldStudentPassword">Current password</label><input id="oldStudentPassword" type="password" required><label for="newStudentPassword">New password</label><input id="newStudentPassword" type="password" minlength="8" required><div class="form-actions"><button class="primary-btn" type="submit">Update password</button></div><p id="passwordChangeMessage" class="form-error"></p></form></section>`;
}
function wardenStudents() {
  return pageHead("Student Records","View students registered in the room management records.")+
  `<section class="panel"><div class="panel-title"><h2>Registered students</h2><span class="pill neutral">${db.students.length} records</span></div><div class="filter-row"><input id="studentSearch" placeholder="Search by student ID or name"></div><div id="studentTable">${studentTable(db.students)}</div></section>`;
}
function studentTable(items) {
  if(!items.length)return `<div class="empty-state">No matching students found.</div>`;
  return `<div class="table-wrap"><table><thead><tr><th>Student ID</th><th>Name</th><th>Room</th><th>Block</th><th>Meal preferences (weekly)</th></tr></thead><tbody>${items.map(s=>{const r=roomForStudent(s.id),prefs=db.meals[String(s.id)]||db.meals[s.id]||[];const b=prefs.filter(x=>x.endsWith("-B")).length,d=prefs.filter(x=>x.endsWith("-D")).length;return `<tr><td>${esc(s.id)}</td><td>${esc(s.name)}</td><td>${r?esc(r.room):"—"}</td><td>${r?esc(r.block):"—"}</td><td>Breakfast: ${b}/7 · Dinner: ${d}/7</td></tr>`;}).join("")}</tbody></table></div>`;
}
function bindPageEvents(root) {
  root.querySelector("#passwordChangeForm")?.addEventListener("submit",async e=>{
    e.preventDefault();
    const account=db.accounts.find(a=>String(a.id)===String(currentUser.id));
    const oldPass=root.querySelector("#oldStudentPassword").value;
    const newPass=root.querySelector("#newStudentPassword").value;
    const message=root.querySelector("#passwordChangeMessage");
    if(!account||account.passwordHash!==await hashPassword(oldPass)){message.textContent="Current password is incorrect.";return;}
    if(newPass.length<8){message.textContent="New password must contain at least 8 characters.";return;}
    account.passwordHash=await hashPassword(newPass);saveDB();message.className="form-success";message.textContent="Password updated successfully.";
    root.querySelector("#passwordChangeForm").reset();
  });
  root.querySelector("#complaintForm")?.addEventListener("submit",e=>{
    e.preventDefault();const idx=Number(root.querySelector("#complaintType").value),room=Number(root.querySelector("#complaintRoom").value);
    if(!Number.isInteger(idx)||!COMPLAINT_TYPES[idx]||!room){toast("Please complete all fields.");return;}
    const type=COMPLAINT_TYPES[idx],c={id:Math.max(0,...db.complaints.map(x=>x.id))+1,studentId:currentUser.id,room,description:type[0],priority:type[1],status:"Pending"};
    db.complaints.push(c);saveDB();renderPage();toast(`Complaint #${c.id} submitted in demo mode.`);
  });
  root.querySelector("#leaveForm")?.addEventListener("submit",e=>{
    e.preventDefault();const reason=root.querySelector("#leaveReason").value.trim();if(!reason){toast("Please enter a reason.");return;}
    db.leaves.push({id:Math.max(0,...db.leaves.map(x=>x.id))+1,studentId:currentUser.id,reason,status:"Pending"});saveDB();renderPage();toast("Leave request submitted in demo mode.");
  });
  root.querySelector("#mealForm")?.addEventListener("submit",e=>{
    e.preventDefault();const day=root.querySelector("#mealDay").value,arr=db.meals[currentUser.id]||[];
    const next=arr.filter(x=>!x.startsWith(`${day}-`));
    if(root.querySelector("#breakfastCheck").checked)next.push(`${day}-B`);
    if(root.querySelector("#dinnerCheck").checked)next.push(`${day}-D`);
    db.meals[currentUser.id]=next;saveDB();toast("Meal selection saved in demo mode.");renderPage();
  });
  root.querySelectorAll("[data-resolve]").forEach(b=>b.addEventListener("click",()=>{
    const id=Number(b.dataset.resolve),c=db.complaints.find(x=>x.id===id);if(!c||c.status==="Resolved")return;
    if(!confirm(`Mark complaint #${id} as resolved?`))return;c.status="Resolved";saveDB();renderPage();toast(`Complaint #${id} marked resolved in demo mode.`);
  }));
  root.querySelectorAll("[data-leave]").forEach(b=>b.addEventListener("click",()=>{
    const id=Number(b.dataset.leave),decision=b.dataset.decision,l=db.leaves.find(x=>x.id===id);if(!l||l.status!=="Pending")return;
    if(!confirm(`${decision} leave request #${id}?`))return;l.status=decision;saveDB();renderPage();toast(`Leave request ${decision.toLowerCase()} in demo mode.`);
  }));
  root.querySelector("#assignRoomForm")?.addEventListener("submit",e=>{
    e.preventDefault();
    const studentId=root.querySelector("#assignStudent").value;
    const roomNo=root.querySelector("#assignRoom").value;
    const student=db.students.find(st=>String(st.id)===String(studentId));
    const room=db.rooms.find(r=>String(r.room)===String(roomNo));
    if(!student||!room){toast("Please select a valid student and room.");return;}
    if(room.students.some(st=>String(st.id)===String(studentId))||roomForStudent(studentId)){toast("This student is already assigned to a room.");return;}
    if(room.students.length>=room.capacity){toast("This room is full. Choose another room.");return;}
    room.students.push({id:student.id,name:student.name});saveDB();renderPage();toast(`${student.name} added to Room ${room.room}.`);
  });
  root.querySelectorAll("[data-remove-room-student]").forEach(b=>b.addEventListener("click",()=>{
    const room=db.rooms.find(r=>String(r.room)===String(b.dataset.removeRoomStudent));
    if(!room)return;
    const student=room.students.find(st=>String(st.id)===String(b.dataset.studentId));
    if(!student)return;
    if(!confirm(`Remove ${student.name} from Room ${room.room}? Their student account will remain registered.`))return;
    room.students=room.students.filter(st=>String(st.id)!==String(b.dataset.studentId));saveDB();renderPage();toast(`${student.name} removed from Room ${room.room}.`);
  }));
  root.querySelectorAll("[data-room-students]").forEach(b=>b.addEventListener("click",()=>{
    const r=db.rooms.find(x=>x.room===b.dataset.room);if(!r)return;
    alert(`Room ${r.room} · Block ${r.block}\nOccupancy: ${r.students.length}/${r.capacity}\n\n${r.students.map(s=>`${s.name} (ID: ${s.id})`).join("\n")||"No occupants"}`);
  }));
  root.querySelector("#studentSearch")?.addEventListener("input",e=>{
    const q=e.target.value.toLowerCase();const filtered=db.students.filter(s=>String(s.id).includes(q)||s.name.toLowerCase().includes(q));
    root.querySelector("#studentTable").innerHTML=studentTable(filtered);
  });
}
renderNav();
